
/////////////////////////////////// INCLUDE ///////////////////////////////////

// For printf(), ...
#include <stdio.h>

// For atoi(), ...
 #include <stdlib.h>

// For write(), close(), read(), ...
#include <unistd.h>

// For open(), ...
#include <fcntl.h>

// For strlen(), strcmp(), ...
#include <string.h>

// For mkfifo(), wait(), ...
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// For perro(), errno, ...
#include <errno.h>

// For assert(), ...
#include <assert.h>

// For isdigit(), ...
#include <ctype.h>

// For SE_FICHIER library
#include "se_fichier.h"

/////////////////////////////////// DEFINE ///////////////////////////////////

#define SIZE_BUF 256
#define TUBE_SERV  "/tmp/tube-serv"
#define TUBE_CLI   "/tmp/tube-cli"

/////////////////////////////////// STRUCT ///////////////////////////////////

typedef struct{
  pid_t PID;
  int num_int;
  int *tab_int;
  int *gain;
}TICKET;

/////////////////////////////////// FUNCTIONS(S) ///////////////////////////////////


/////////////////////////////////// TOOLS STRING CRYPTED ///////////////////////////////////

/** Format: B.YYYY.XXXX.Z.[n1, n3, ..., nZ].W/T.G.E
  * B: marque début du message
  * YYYY: qui suis je ? SERV ou CLIE
  * XXXX: le PID (size unknown)
  * Z: nombre de numéro joué
  * [n1, n3, ..., nZ]: les numéros
  * W: nombre de numéro correct (win)
  * T: nuombre de numéro max
  * G: les gains
  * E: marque fin de message
  */

// Create a formatting string with all information about the ticket in only one string
// \param format      the empty string formated
// \param who_i_am    for write who send the msg
// \param my ticket   information about the client ticket
// \return format     the string filled
char *formatting(char *format, char *who_i_am, TICKET my_ticket) {
  char me[4] = "\0";

  if( !strcmp( who_i_am, "client" ) )
    strcat(me, "CLIE\0");
  else if( !strcmp( who_i_am, "server" ) )
    strcat(me, "SERV\0");
  else
    exit(EXIT_FAILURE);

  sprintf( format, "B.%s.%d.%d.[", me, my_ticket.PID, my_ticket.num_int );

  char tmp[8];
  for( int i = 0 ; i < my_ticket.num_int ; i++ ) {
    sprintf( tmp, "%d, ", my_ticket.tab_int[i] );
    strcat( format, tmp );
  }

  strcat(format, "].0/0.0.E\0");

  return format;
}

// Found the user pid in 'format'
// \param format    crypted string
// \return          user pid
int _get_pid(char *format) {
  // size < 8 numbers
  char tmp[8]; int i = 0;

  //B.YYYY. = 6, +1 for the first pid number
  int c = 7;
  while( isdigit( format[c] ) ) {
    tmp[i] = format[c];
    i++; c++;
  }
  tmp[i] = '\0';

  return atoi( tmp );
}

// Found the count of number int 'format'
// \param format    crypted string
// \return          count of number
int _get_count(char *format) {
  // B.YYYY.XXXX.Z.[ ]
  int c = 0;
  while( format[c]  !=  '[' ) c++;

  return atoi( &format[c-2] );
}

// Found the client's numbers in 'format' and filled 'tab_int'
// \param format      crypted string
// \param tab_int     tab with number
void _get_client_number(int *tab_int, char *format) {
  char tmp[4]; int tmp_i, tab_i = 0, count = 0;

  // B.YYYY.XXXX.Z.[ ]
  int num = _get_count( format );

  int c = 0;
  while( format[c]  !=  '[' ) c++;
  c+=1;
  while( count <= num ) {
    tmp[0] = '\0'; tmp_i = 0;
    while( isdigit( format[c] ) ) {
      tmp[ tmp_i ] = format[c];
      tmp_i++; c++;
    }
    tab_int[ tab_i ] = atoi( tmp );
    tab_i++;
    count++;
    while( !isdigit( format[c] ) ) c++;
  }
}

// Found the gain in 'format'
// \param format      crypted string
// \return gain       the gain for the client
int _get_gain(char *format) {
  // B.YYYY.XXXX.Z.[n1, n3, ..., nZ].W/T.G.E
  int gain = 0;

  int c = 0;
  while( format[c]  !=  '/' ) c++;
  c+=3;

  char tmp[16]; int tmp_i = 0;
  while( isdigit( format[c] ) ) {
    tmp[ tmp_i ] = format[c];
    tmp_i++; c++;
  }

  gain = atoi( tmp );

  return gain;
}


// Check the format of the message
// \param request     the message receive or send
// \return            0 or 1
int good_format_msg(char *request) {
  return ( request[0]  ==  'B'  &&
           request[ strlen( request ) - 1 ]  ==  'E' );
}

// Check the client's numbers in 'request' is good
// \param win           the server ticket with the good number
// \param request       the client ticket
// \param[out] count    count of number
// \param[out] gain     client gain
void check_data_base(TICKET win, char *request, int *count, int *gain) {
  *count = 0; *gain = 0;
  // The count of client's number is less than 16
  int *tab = calloc( 16, sizeof(int) );
  _get_client_number( tab, request );

  // Compare with data base
  for( int i = 0 ; i < win.num_int ; i++ ) {
    for( int j = 0 ; j < _get_count( request ) ; j++ ) {
      if( win.tab_int[i]  ==  tab[j] ) {
        *count+=1;
        *gain = win.gain[*count];
      }
    }
  }

  free(tab);
}

// Treatement of 'resquest'
// \param win       the server ticket with the good number
// \param request   the client ticket
int number_win(TICKET win, char *request) {
  if( _get_count( request ) !=  win.num_int ) {
    request[ strlen( request ) - 7] = '\0';
    strcat( request, "0/0.0.E" );
    printf("......Error\n\t\t\tYou haven't play the good count of number\n");
    return -1;
  }

  int count, gain;
  check_data_base(win, request, &count, &gain);

  // Change the request with the new gain and number of good numbers
  char tmp[16]; request[ strlen( request ) - 7] = '\0';
  sprintf( tmp, "%d/%d.%d.E", count, win.num_int, gain );
  strcat( request, tmp );

  return 0;
}

// Check is client win all recompense
// \param request     information about client
// \return            1 or 0
int is_jackpot(char *request) {
  int c;

  // Skip the first part of the 'request' until the good number
  for( c = strlen( request ) ; c > 0 ; c-- )
    if( request[c]  ==  ']' ) break;
  c+=2;

  // if is jackpot, 'request[c]'  ==  'request[c+2]', return 1
  return ( ( request[c]  ==  request[c+2] )  &&
           ( request[c]  !=  '0' ) );
}

/////////////////////////////////// END TOOLS STRING CRYPTED ///////////////////////////////////


/////////////////////////////////// SEND/ RECEIVE FIFO ///////////////////////////////////

// Open the fifo and write a msg
// \param tube_name   the tube for write
// \param msg         the msg which need to write
// \return            -1 if failed, 0 end of file,
//                      else number of inputs
int send_msg(const char *tube_name, char *msg) {
  SE_FICHIER file;

  int success = 0;
  file = SE_ouverture ( tube_name, O_WRONLY );

  // Check if we send a crypted message
  if( !good_format_msg( msg ) ) {
    printf("In million.c: Error with 'send_msg()': invalid format of the message\n");
    exit( EXIT_FAILURE );
  }

  // read char by char until the end of the file,
  // and if rest char in message
  int c = 0;
  do {
    success =  SE_ecritureCaractere( file, msg[c] );
    c++;
  } while( success  >  0  &&  c  <  strlen( msg ));

  SE_fermeture ( file );
  return success;
}

// Open the fifo and read a msg
// \param tube_name   the tube for write
// \param msg         the msg which need to write
// \return            -1 if failed, 0 end of file,
//                      else number of inputs
int receive_msg(const char *name_tube, char *msg_read) {
  SE_FICHIER file;
  int success = 0;

  file = SE_ouverture( name_tube, O_RDONLY );

  // read char by char until the end of the file
  int c = 0;
  do {
    success = SE_lectureCaractere( file, &msg_read[c] );
    c++;
  } while( success  >  0 );

  SE_fermeture( file );
  return success;
}

// Create a fifo
// \param name_tube     the name of the fifo
// \return              0 on succuss, else errno
int creat_fifo(const char *name_tube) {
  if( mkfifo( name_tube, 0666 )  ==  -1 ) {
    perror("In million.c: Error with 'mkfifo()' in 'creat_fifo()'\n");
    return errno;
  }
  return 0;
}

/////////////////////////////////// END FIFO ///////////////////////////////////


/////////////////////////////////// TICKET ///////////////////////////////////


// Init a ticket for the client
// \param pid     the client's pid
// \param n_int   the number of elements in t_int
// \param t_int   the table of elements
// \return        a ticket
TICKET ticket_client_init(pid_t PID, int n_int, int *t_int) {
  TICKET my_ticket;

  my_ticket.PID = PID;
  my_ticket.num_int = n_int;

  my_ticket.tab_int = malloc( n_int * sizeof(int) );
  assert( my_ticket.tab_int  !=  NULL );

  for ( int i = 0 ; i < n_int;  ++i )
    my_ticket.tab_int[i] = t_int[i];

  my_ticket.gain = calloc( my_ticket.num_int + 1, sizeof(int) );
  assert( my_ticket.gain  !=  NULL );

  return my_ticket;
}

// Init a ticket for the server with the data base
// \param pid         the server's pid
// \param name_file   the file of the data base
// \return            a ticket
TICKET ticket_win_init(pid_t PID, const char *name_file) {
  SE_FICHIER file;
  TICKET win;

  win.PID = PID;

  int success = 0;
  file = SE_ouverture( name_file, O_RDONLY );

  // Read how much number we need
  success = SE_lectureEntier( file, &win.num_int );
  assert( success  !=  -1 );

  // Init tab for save its numbers
  win.tab_int = malloc( win.num_int * sizeof(int) );
  assert( win.tab_int  !=  NULL );

  // Read numbers
  for( int i = 0 ; i < win.num_int ; i++ ) {
    success = SE_lectureEntier( file, &win.tab_int[i] );
    assert( success  !=  -1 );
  }

  win.gain = calloc( win.num_int + 1, sizeof(int) );
  assert( win.gain  !=  NULL );

  win.gain[0] = 0;

  // Read numbers and gains
  int tmp;
  for( int i = win.num_int ; i > 0 ; --i ) {
    success = SE_lectureEntier(file, &tmp);
    assert( success  !=  -1 );

    success = SE_lectureEntier(file, &win.gain[i]);
    assert( success  !=  -1 );
  }

  SE_fermeture(file);

  return win;
}

// Print the ticket
// \param my_ticket   the ticket which need to print
// \param win         if win = 0, is the ticket's server,
//                              else the ticket's client
void ticket_print(TICKET my_ticket, int win) {
  printf("PID: %d\n", my_ticket.PID);

  if( win ) printf("Ticket winner:\n");
  printf("Number of int: %d\n", my_ticket.num_int);

  if( win ) printf("Num: ");
  else printf("My num:");
  for( int i = 0 ; i < my_ticket.num_int ; i++ )
    printf(" %d ", my_ticket.tab_int[i]);

  printf("\n");
  for( int i = 0 ; i < my_ticket.num_int ; i++)
    printf("%d\n", my_ticket.gain[i] );
}

// Release the ticket
// \param ticket    ticket to release
void ticket_end(const char *who_i_am, TICKET ticket) {
  if( !strcmp( who_i_am, "client" ) ) printf("Client n°%d: Ticket releases", ticket.PID);
  else if( !strcmp( who_i_am, "server" ) ) printf("Server n°%d: Ticket releases", ticket.PID);
  else printf("Who are you ?\n");

  free(ticket.tab_int);
  free(ticket.gain);

  printf("..............OK\n");
}

/////////////////////////////////// END TICKET ///////////////////////////////////


/////////////////////////////////// RUN ///////////////////////////////////

// Take argv and return a int table with client's numbers
// \param argc        count of number in 'argv'
// \param argv        command line
// \param [out] tab   int table with clien't number
void treatment_argv(int argc, const char *argv[], int *tab) {
  // Cast
  int num_client;
  for( int i = 2 ; i < argc ; i++ ) {
    if( isdigit( *argv[i] ) ) {
      num_client = atoi( argv[i] );

      // Lottery number always > 0
      if( num_client  >  0 )
        tab[i-2] = num_client;

      else {
        printf("In million.c: Error in 'treatment_argv()' l.465: invalid number\nNumber replace to 0\n");
        tab[i-2] = 0;
      }
    }
    else {
      printf("In million.c: Error in 'treatment_argv()' l.470: invalid number\nNumber replace to 0\n");
      tab[i-2] = 0;
    }
  }
}

// run client
// \param argc     count of number in 'argv'
// \param argv     command line
// \return         2 if server is close
//                 1 if the client touch the jackpot
//                 0 else
int run_client(int argc, const char *argv[]) {
  TICKET my_ticket;

  // Save numbers in argv[]
  int *tab = malloc( (argc - 2) * sizeof(int) );
  assert( tab  !=  NULL );
  treatment_argv( argc, argv, tab );

  my_ticket = ticket_client_init( (int)getpid(), argc - 2, tab );
  free(tab);

  printf("\nClient n°%d: Send in process.....", (int)getpid());
  char format[SIZE_BUF];
  sleep(1);
  if( send_msg( TUBE_SERV,
    formatting( format, "client", my_ticket ) )  ==  -1 ) {
    printf(".....ERROR:\n\t\tThere is no ongoing lottery\n");
    ticket_end( "client", my_ticket );
    return 2;
  }
  else
    printf(".........OK\n");

  char msg[SIZE_BUF] = "\0";
  printf("Client n°%d: reception in process...", my_ticket.PID);
  sleep(1);
  if( receive_msg( TUBE_CLI, msg )  >=  0 )
    printf("......OK\n");
  else
    printf("...ERROR\n");
  SE_suppression( TUBE_CLI );

  int gain = _get_gain( msg );

  if( gain )
    printf("Client n°%d: Great ! You have win: %d $\n", my_ticket.PID, gain);
  else if( gain  ==  0 )
    printf("Client n°%d: Sorry you have lost, try again !\n", my_ticket.PID);
  else
    printf("In million.c: Error with run_client(): invalid gain\n");

  ticket_end( "client", my_ticket );

  if( is_jackpot( msg ) )
    return 1;
  return 0;
}

// Treat the message send by the client
// \param request     the message send by the client
// \param  win        the server data base
void treatment_serv(char *request, TICKET win) {
  // Compare the number with the data base
  if( !number_win( win, request ) )
    printf("......OK\n");

  creat_fifo( TUBE_CLI );
  printf("Server n°%d: Send in process...", win.PID);
  sleep(1);
  if( send_msg( TUBE_CLI, request )  >  0 ) {
    printf("...........OK\n");
  }
}

// Run server
// \param path_file   the data base file
int run_server(const char *path_file) {
 TICKET win;
 win = ticket_win_init( (int)getpid(), path_file );

 creat_fifo( TUBE_SERV );

  while( 1 ) {

    char request[SIZE_BUF] = "\0";
    if( receive_msg( TUBE_SERV, request )  !=  -1 ) {

      printf("Server n°%d: A wild client appears\n", win.PID);
      printf("Server n°%d: Treatment in process...", win.PID);
      treatment_serv(request, win );

      if( is_jackpot( request ) ) {
        printf("A client touch the jackpot ! End of lottery\n");
        break;
      }
    }
  }

  SE_suppression( TUBE_SERV );
  ticket_end( "server", win );

  return 0;
}

/*##### MAIN #####*/

// test arguments
// \param argc    number of args
// \param argv    list of args
// \return        0 on success, else 1
int test_argv(int argc, char const *argv[]) {
  if( argc  <  2 ) {
    printf("Error in 'million.c': need argument(s) to compile\n");
    return 1;
  }

  // ./million server file.cfg
  if( !strcmp( argv[1], "server" ) ) {
    if( argc  !=  3 ) {
      printf("Error in 'million.c': need a file to compile\n");
      return 1;
    }
  }

  // ./million client n1 n2 ... nx
  else if( !strcmp( argv[1], "client" ) ) {
    // Need at least one number
    if( argc < 3 ) {
      printf("Error in 'million.c': need many number(s) to compile\n");
      return 1;
    }
  }

  return 0;
}

int main(int argc, char const *argv[])
{
  int success;
  if( !test_argv(argc, argv) ) {

    if( !strcmp( argv[1], "server" ) ) {
      printf("\nServer n°%d: Launching....................OK\n\n", (int)getpid() );
      success = run_server( argv[2] );
      printf("\nServer n°%d: Ending.......................OK\n\n", (int)getpid() );
      return success;
    }
    else if( !strcmp( argv[1], "client" ) ) {
      printf("\nClient n°%d: Launching....................OK\n\n", (int)getpid() );
      success = run_client( argc, argv );
      printf("\nClient n°%d: Ending.......................OK\n", (int)getpid() );
      return success;
    }
 }
}
