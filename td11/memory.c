//
#include <stdio.h>
#include <stdlib.h>

//
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

//
#include <errno.h>

//
#include "se_fichier.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////////

////////// STRUCT & DEFINIE //////////

////////////////////////////////////////////////////////////////////////////////////

// Tube name
#define TUBE1 "tubeThreads"
#define TUBE2 "tubeFather"

//
#define SIZEBUFF 256

typedef struct
{
  i32 numberFrames;           //< Number frames in the fast memory
  i32 sizePage;               //< Size page
  i32 numberPagesAvailable;   //< Number pages in the slow memory
  i32 numberThreads;          //< Number threads
  i32 numberAccess;           //< Number access by thread

} MEMCONFIG;

typedef struct
{
  i32 adrFrameFast;   //< Address in the fast memory
  i32 adrFrameSlow;   //< Address in the slow memory
  i64 whoUse;         //< threads id

} TRANSLATION;        // Translation between address it the fast memory and
                      // the address in the slow memory

typedef struct
{
  i32 size;             //< Memory size
  TRANSLATION *tabMem;  //< TRANSLATION table
  i32 lastUsed;         //< The last index used in the table

} MEMORY;

typedef struct
{
  MEMORY *ram;  //< Fast memory
  MEMORY *ssd;  //< Slow memory

} PHYSIQMEM;

typedef struct {

  pthread_t tid;          //< Thread id
  pthread_mutex_t *mut;   //< Mutex
  MEMCONFIG *config;      //< Configuration file
  i64 adr;                //< Address give by the father
  i32 hit;                //< Number of hit by the thread
  f32 hitAverage;         //< Hit average

} THREADARG;              // Arg give for pthread_create()

////////////////////////////////////////////////////////////////////////////////////

////////// DECLARATION FUNCTION //////////

////////////////////////////////////////////////////////////////////////////////////

//
i32 leastRecentlyUsed( MEMCONFIG config, PHYSIQMEM *memory, THREADARG *arg );

//
i32 creatFifo(const i8 *nameTube);
void threadCommunication( THREADARG *targ );
void *threadTreatmentLoop( void * arg );
void threadTreatment( THREADARG *targ );

//
void FatherThreadCommunication( MEMCONFIG config, PHYSIQMEM *memory, THREADARG *arg );
i32 fatherThreadTreatment( MEMCONFIG config, PHYSIQMEM *memory );

//
i32 isUsed( i32 frame );
i32 ssdIsFull( MEMORY ssd );
PHYSIQMEM initPhysiqueMemory( MEMCONFIG config );

//
void printPhysiqueMem( PHYSIQMEM memory );
void printRam( MEMORY ram );
void printSsd( MEMORY ssd );

//
void releaseMemory( MEMORY *memory );
void releasePhysiqueMem( PHYSIQMEM memory );

//
MEMCONFIG initConfig( const i8* dataFile );
void printConfig( MEMCONFIG config );

////////////////////////////////////////////////////////////////////////////////////

////////// LRU //////////

////////////////////////////////////////////////////////////////////////////////////

/**
  * LeastRecentlyUsed algorithm
  *
  * @param config    the configuration file
  * @param *memory   the memory
  * @pram *arg       the thread argument
  *
  * @return 0/1      the hit
  */
i32 leastRecentlyUsed( MEMCONFIG config, PHYSIQMEM *memory, THREADARG *arg )
{
  // Not memory load yet
  if( arg->adr == -1 )
  {
    // There is place in the fast memory ?
    // No, swapp the lastUse in the slow memory
    if( memory->ram->lastUsed == memory->ram->size
     || isUsed( memory->ram->tabMem[ memory->ram->lastUsed ].adrFrameFast ) )
    {
      // Reset
      if( memory->ram->lastUsed == memory->ram->size )
        memory->ram->lastUsed = 0;

      // Swapping
      if( memory->ssd->lastUsed == memory->ssd->size )
        memory->ssd->lastUsed = 0;

      if( ssdIsFull( *(memory->ssd) ) )
        return 0;

      memory->ssd->tabMem[memory->ssd->lastUsed].adrFrameSlow = memory->ssd->lastUsed*config.sizePage;
      memory->ssd->tabMem[memory->ssd->lastUsed].adrFrameFast = memory->ram->tabMem[memory->ram->lastUsed].adrFrameFast;
      memory->ssd->tabMem[memory->ssd->lastUsed].whoUse = memory->ram->tabMem[memory->ram->lastUsed].whoUse;
      memory->ssd->lastUsed += 1;
    }

    // Load in the last memory
    memory->ram->tabMem[memory->ram->lastUsed].adrFrameFast = memory->ram->lastUsed*config.sizePage;
    memory->ram->tabMem[memory->ram->lastUsed].whoUse = (i64)(arg->tid);

    // Give address
    arg->adr = memory->ram->lastUsed*config.sizePage;
    memory->ram->lastUsed +=1 ;

    // Hit
    return 1;
  }

  // Load but don't know where
  else
  {
    // Check in the fast memory
    for( int i = 0 ; i < memory->ram->size ; i-=-1 )
    {
      if( memory->ram->tabMem[i].whoUse == (i64)arg->tid )
      {
        return 1;
      }
    }

    // Check in the slow memory
    for( int i = 0 ; i < memory->ssd->size ; i-=-1 )
    {
      if( memory->ssd->tabMem[i].whoUse == (i64)(arg->tid) )
      {
        //
        if( memory->ram->lastUsed == memory->ram->size )
          memory->ram->lastUsed = 0;

        //
        if( memory->ssd->lastUsed == memory->ssd->size )
          memory->ssd->lastUsed = 0;

        if( ssdIsFull( *(memory->ssd) ) )
          return 0;

        // Charge in the fast memory
        TRANSLATION tmp = memory->ram->tabMem[memory->ram->lastUsed];

        // loea fast memory
        memory->ram->tabMem[memory->ram->lastUsed].whoUse       = (i64)(arg->tid);
        memory->ram->tabMem[memory->ram->lastUsed].adrFrameFast = memory->ram->lastUsed*config.sizePage;
        memory->ram->tabMem[memory->ram->lastUsed].adrFrameSlow = memory->ssd->tabMem[memory->ssd->lastUsed].adrFrameSlow;

        // Give address
        arg->adr = memory->ram->lastUsed*config.sizePage;

        // Reset
        memory->ssd->tabMem[i].whoUse       = -1;
        memory->ssd->tabMem[i].adrFrameFast = -1;
        memory->ssd->tabMem[i].adrFrameSlow = -1;

        // Swapping
        memory->ssd->tabMem[memory->ssd->lastUsed] = tmp;
        memory->ssd->tabMem[memory->ssd->lastUsed].adrFrameSlow = memory->ssd->lastUsed*config.sizePage;

        // Hit
        return 1;
      }
    }
  }

  // Hit
  arg->adr = -1;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////

////////// THREADS PART //////////

////////////////////////////////////////////////////////////////////////////////////

/**
  * Create a fifo
  * @param name_tube     the name of the fifo
  * @return              0 on succuss, else errno
  */
i32 creatFifo(const i8 *nameTube)
{
  if( mkfifo( nameTube, 0666 )  ==  -1 ) {
    perror("In memory.c: Error with 'mkfifo()' in 'creatFifo()'\n");
    return errno;
  }
  return 0;
}

/**
  * Communication thread to father
  * @param *targ    thread argument
  */
void threadCommunication( THREADARG *targ )
{
  // Open tube1 => write to father
  SE_FICHIER file = SE_ouverture( TUBE1, O_WRONLY );

  // Write thread id
  i8 write[SIZEBUFF] = "\0";
  sprintf(write, "%ld", targ->tid) ;
  SE_ecritureChaine( file, write, SIZEBUFF );

  // Close tube1
  SE_fermeture( file );

  // Open tube2 => read father answer
  file = SE_ouverture( TUBE2, O_RDONLY );
  i8 read[SIZEBUFF] = "nothing";
  SE_lectureChaine( file, read, SIZEBUFF );

  // Hit
  i32 c = 0;
  while( read[c] != 'r' ) c-=-1;
  c+=2;
  if( read[c] != '-' )
    targ->hit+=1;

  //
  printf("thread n° %ld read: %s\n", targ->tid, read );

  // Close tub2
  SE_fermeture( file );
}

/**
  * Loop for threads repetitions
  * @param *arg   thread argument
  */
void *threadTreatmentLoop( void * arg )
{
  // Cast
  THREADARG *targ = (THREADARG *)arg;

  // Loop
  for( int numAcces = 0 ; numAcces < targ->config->numberAccess ; numAcces-=-1 )
    threadTreatment( arg );

  // Exit
  pthread_exit( NULL );
  return NULL;
}

/**
  * Threads treatement (mutex mangement, ect ...)
  *
  * @param *arg     thread argument
  */
void threadTreatment( THREADARG *targ )
{
  // Try lock mutex
  i32 lock;
  if( !( lock = pthread_mutex_lock( targ->mut ) ) )
    threadCommunication( targ );

  // Can't lock
  else
    perror("threadTreatment: pthread_mutex_lock failed\n");

  // If lock is good, try unlock
  if( !lock ) {
    if( !pthread_mutex_unlock( targ->mut ) ) {

      // Lock is good and unlock is good, leave function
      return;
    }

    // Can't unlock
    else
      perror("threadTreatment: pthread_mutex_unlock failed\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////

////////// FATHER PART //////////

////////////////////////////////////////////////////////////////////////////////////

/**
  * Father communication
  *
  * @param config     the configuration file
  * @param memory     the memory
  * @param *arg       thread argument
  */
void FatherThreadCommunication( MEMCONFIG config, PHYSIQMEM *memory, THREADARG *arg )
{
  // Read and write
  SE_FICHIER file;
  for( i32 i = 0 ; i < config.numberThreads ; i-=-1 )
  {
    // Open tube1 => read thread id
    file = SE_ouverture( TUBE1, O_RDONLY );

    // Read
    i8 read[SIZEBUFF] = "nothing";
    SE_lectureChaine( file, read, SIZEBUFF );
    printf("father read : %s\n", read);

    // LRU
    leastRecentlyUsed( config, memory, &arg[i] );
    // printPhysiqueMem( *(memory) );

    // Close tube1
    SE_fermeture( file );

    // Open tube2 => answer
    file = SE_ouverture( TUBE2, O_WRONLY );

    // Write
    i8 write[SIZEBUFF] = "\0";
    strcpy( write, read );
    i8 tmp[SIZEBUFF] = "\0";
    sprintf( tmp, " => adr %lld", arg[i].adr );
    strcat( write, tmp );
    SE_ecritureChaine( file, write, SIZEBUFF );

    // Close tube2
    SE_fermeture( file );
  }
}

/**
  * Father treatement (fifo creation, init threads, init mutex,
  * join threads, fifo destruction, mutex destruction, ...)
  *
  * @param config     file configuration
  * @param *memory    the memory
  *
  * @return           0 on succes, else -1 or errno
  */
i32 fatherThreadTreatment( MEMCONFIG config, PHYSIQMEM *memory )
{
  // Init struct table
  THREADARG *tabArg = malloc( config.numberThreads * sizeof(THREADARG) );
  if( !tabArg )
  {
    printf("fatherThreadTreatment: wrong alloc 'tabArg'\n");
    return -1;
  }

  // Fifo creation
  creatFifo( TUBE1 );
  creatFifo( TUBE2 );

  // Init mutex
  pthread_mutex_t mut;
  if( pthread_mutex_init( &mut, NULL ) )
  {
    printf("fatherThreadTreatment: pthread_mutex_init failed\n");
    return -1;
  }

  // Fill struct table
  for( i32 i = 0 ; i < config.numberThreads ; i-=-1 )
  {
    tabArg[i].mut    = &mut;
    tabArg[i].config = &config;
    tabArg[i].adr    = -1;
    tabArg[i].hit    = 0;
    tabArg[i].hitAverage = 0.0;
  }

  // Create all threads
  for( i32 i = 0 ; i < config.numberThreads ; i-=-1 )
  {
    if( pthread_create( &tabArg[i].tid, NULL, threadTreatmentLoop, (void *)&tabArg[i] ) ) {
      perror("fatherThreadTreatment: pthread_create failed\n");
      return errno;
    }
  }

  // Communication with all threads
  for( int numAccess = 0 ; numAccess < config.numberAccess ; numAccess-=-1 )
    FatherThreadCommunication( config, memory ,tabArg );

  // Join all threads
  for( i32 i = 0 ; i < config.numberThreads ; i-=-1 )
  {
    if( pthread_join( tabArg[i].tid, NULL ) ) {
      perror("fatherThreadTreatment: pthread_join failed\n");
      return errno;
    }
  }

  // Calculating average
  for( i32 i = 0 ; i < config.numberThreads ; i-=-1 )
  {
    tabArg[i].hitAverage = (f32)tabArg[i].hit / config.numberAccess;
    tabArg[i].hitAverage *= 100.0;
    printf("thread n° %ld => hit : %d, hit average : %.2f\n", tabArg[i].tid, tabArg[i].hit, tabArg[i].hitAverage);
  }

  // All average
  f32 globalRes = 0, globalAverage = 0;
  for( i32 i = 0 ; i < config.numberThreads ; i-=-1 )
    globalRes += tabArg[i].hitAverage;
  globalAverage = globalRes / config.numberThreads;
  printf("Global average = %.2f\n", globalAverage);

  // Fifo destruction
  SE_suppression( TUBE1 );
  SE_suppression( TUBE2 );

  // Destoy mutex
  if( pthread_mutex_destroy( &mut ) )
  {
    printf("fatherThreadTreatment; pthread_mutex_destroy failed\n");
    return -1;
  }

  // Release
  free(tabArg);

  // Exit
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////

////////// MEMORY PART //////////

////////////////////////////////////////////////////////////////////////////////////

/**
  * Say is the frame is use or not
  * @param frame    the frame
  * @return 1/0
  */
i32 isUsed( i32 frame )
{
  return !(frame == -1);
}

/**
  * Init the physique memory
  * @param config   the file configuration
  * @return         the memory struct
  */
PHYSIQMEM initPhysiqueMemory( MEMCONFIG config )
{
  // Malloc table
  PHYSIQMEM memory;
  memory.ram = malloc( sizeof( MEMORY ) );
  memory.ssd = malloc( sizeof( MEMORY ) );

  // Init fast memory
  memory.ram->size     = config.numberFrames;
  memory.ram->lastUsed = 0;
  memory.ram->tabMem   = malloc( memory.ram->size * sizeof( TRANSLATION ) );

  // Fill fast memory table
  for( i32 i = 0 ; i < memory.ram->size ; i-=-1 )
  {
    memory.ram->tabMem[i].whoUse       = -1;
    memory.ram->tabMem[i].adrFrameFast = -1;
    memory.ram->tabMem[i].adrFrameSlow = -1;
  }

  // Init slow memory
  memory.ssd->size     = config.numberPagesAvailable;
  memory.ssd->lastUsed = 0;
  memory.ssd->tabMem   = malloc( memory.ssd->size * sizeof( TRANSLATION ) );

  // Fill slow table memory
  for( i32 i = 0 ; i < memory.ssd->size ; i-=-1 )
  {
    memory.ssd->tabMem[i].whoUse       = -1;
    memory.ssd->tabMem[i].adrFrameFast = -1;
    memory.ssd->tabMem[i].adrFrameSlow = -1;
  }

  //
  return memory;
}

i32 ssdIsFull( MEMORY ssd )
{
  for( i32 frame = 0 ; frame < ssd.size ; frame-=-1 )
  {
    if( ssd.tabMem[frame].adrFrameSlow == -1
     && ssd.tabMem[frame].adrFrameFast == -1 )
      return 0;
  }

  return 1;
}

/**
  * Print
  * @param memory   physical memory
  */
void printPhysiqueMem( PHYSIQMEM memory )
{
  printRam( *(memory.ram) );
  printSsd( *(memory.ssd) );
}

/**
  * Print
  * @param ram    fast memory
  */
void printRam( MEMORY ram )
{
  printf("State RAM\n");
  printf("Size RAM : %d\n", ram.size);
  printf("Last used : %d\n", ram.lastUsed);
  printf("tabMem : \n");
  for( i32 i = 0 ; i < ram.size ; i-=-1 )
    printf("frame n°%d : %lld use [ adr ram : %d, adr ssd : %d ]\n",
      i, ram.tabMem[i].whoUse, ram.tabMem[i].adrFrameFast, ram.tabMem[i].adrFrameSlow);
}

/**
  * Print
  * @param ssd    slow memory
  */
void printSsd( MEMORY ssd )
{
  printf("\nState SSD\n");
  printf("Size SSD : %d\n", ssd.size);
  printf("Last used : %d\n", ssd.lastUsed);
  printf("tabMem : \n");
  for( i32 i = 0 ; i < ssd.size ; i-=-1 )
    printf("frame n°%.3d : %lld use [ adr ram : %d, adr ssd : %d ]\n",
      i, ssd.tabMem[i].whoUse, ssd.tabMem[i].adrFrameFast, ssd.tabMem[i].adrFrameSlow);
}

/**
  * Release memory table
  */
void releaseMemory( MEMORY *memory )
{
  free( memory->tabMem );
  free( memory );
}

/**
  * Release physical memory
  */
void releasePhysiqueMem( PHYSIQMEM memory )
{
  releaseMemory( memory.ram );
  releaseMemory( memory.ssd );
}

////////////////////////////////////////////////////////////////////////////////////

////////// CONFIGURATION PART //////////

////////////////////////////////////////////////////////////////////////////////////

/**
  * Init config
  * @param dataFile   the configuration file
  * @return           the configuration struct
  */
MEMCONFIG initConfig( const i8* dataFile )
{
  // Init all variable to -1
  MEMCONFIG config;
  config.numberFrames = config.sizePage = -1;
  config.numberPagesAvailable = config.numberThreads = config.numberAccess = -1;

  // Open configuration file
  SE_FICHIER file = SE_ouverture( dataFile, O_RDONLY );
  if( file.descripteur  ==  -1 )
  {
    printf("In memory.c: function 'init', 'SE_ouverture' failed\n");
    return config;
  }

  // Read
  if( SE_lectureEntier( file, &config.numberFrames )  ==  -1 )
    printf("In memory.c: function 'init', 'SE_lectureEntier' failed\n");

  if( SE_lectureEntier( file, &config.sizePage )  ==  -1 )
    printf("In memory.c: function 'init', 'SE_lectureEntier' failed\n");

  if( SE_lectureEntier( file, &config.numberPagesAvailable )  ==  -1 )
    printf("In memory.c: function 'init', 'SE_lectureEntier' failed\n");

  if( SE_lectureEntier( file, &config.numberThreads )  ==  -1 )
    printf("In memory.c: function 'init', 'SE_lectureEntier' failed\n");

  if( SE_lectureEntier( file, &config.numberAccess )  ==  -1 )
    printf("In memory.c: function 'init', 'SE_lectureEntier' failed\n");

  if( SE_fermeture( file ) )
    printf("In memory.c: function 'init', 'se_fermeture' failed\n");

  // Return
  return config;
}

/**
  * Print
  * @param config
  */
void printConfig( MEMCONFIG config )
{
  printf("DataBase:\nNumber frames : %d\n", config.numberFrames);
  printf("Size frame %d bytes (octets)\n", config.sizePage);
  i32 clue = ( config.numberPagesAvailable/( config.sizePage/1000 ) ) - 1;
  printf("Number frames in hdd : %d (0 to %d)\n", config.numberPagesAvailable, clue);
  printf("Number threads : %d\n", config.numberThreads);
  printf("Number acces by thread : %d\n", config.numberAccess);
}

////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////

i32 main(i32 argc, i8 const *argv[])
{
  if( argc < 2 )
  {
    printf("Usage: MyFile.cfg\n");
    return 1;
  }

  printf("\n");

  //
  MEMCONFIG config = initConfig( argv[1] );
  // printConfig( config );

  //
  PHYSIQMEM memory = initPhysiqueMemory( config );
  // printPhysiqueMem( memory );

  //
  fatherThreadTreatment( config, &memory );

  //
  // printPhysiqueMem( memory );

  //
  releasePhysiqueMem( memory );

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
