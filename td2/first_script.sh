doss_is_creat (){
  # Function which test if the directory exist.
  if [ -d $1 ]
  then
  rm -r $1
  fi
}

file_is_creat (){
  # Function which test if the file exist.
  if [ -f $1 ]
  then
  rm $1
  fi
}

creat_dir (){
  # Function which creat 'dir'.
  doss_is_creat $1
  echo "directory '$1' is creat."
  mkdir $1
}

creat_project (){
  # Function which creat all dir 'project'.
  creat_dir $1
  creat_dir $1/$2
  creat_dir $1/$3
  creat_dir $1/$4
}

creat_file (){
 # Function which creat a file.
 echo "File '$1' is creat."
  file_is_creat $1
  touch $1
}

creat_fic_and_write (){
  # Function which write in a file.
  creat_file $1/$2
  echo $3 > $1/$2
}

creat_tar (){
  doss_is_creat $2
  tar -zcvf $2.tar.gz $1
}

all_creat (){
  creat_project /tmp/project doc include src
  creat_fic_and_write /tmp/project README FORCEUR_nounours
  creat_file /tmp/project/include/func.h
  creat_file /tmp/project/src/main.c
  creat_file /tmp/project/src/func.c
  creat_file /tmp/project/contents.txt
  ls -R /tmp/project > /tmp/project/contents.txt
}

copy (){
  echo "Copy of /tmp/project in /tmp/projectV2"
  cp -r /tmp/project /tmp/projectV2
}

dell (){
  echo "Dell of /tmp/project"
  rm -r /tmp/project
}

exo2_3 (){
  all_creat
  copy
  dell
  creat_tar /tmp/projectV2 /tmp/pv2
}

### main ###
exo2_3
