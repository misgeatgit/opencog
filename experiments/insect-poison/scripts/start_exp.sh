#!/bin/bash

oc_dir=/home/misgana/OPENCOG
exp_dir=$oc_dir/opencog/experiments/insect-poison

log_dir=$exp_dir/data/log
if [ "$(ls $log_dir)" ]; then
	read -p $'It appears that $log_dir contains some files.
	What do you want to do  with it
       	1. Delete
       	2.Move\n' choice;
fi
if [ "$choice" = "1" ]
then
	rm -rf $log_dir/*
	echo "Deleted all files in "$log_dir
elif [ "$choice" = "2" ]
then
	#shopt -s direxpand
	#shopt -s cdable_vars
	read -r -e -p $'Please specify directory to be created.\n' dir
	path=$(eval echo $dir)
	echo "Creating dir $path"
	mkdir -p $path
	mv $log_dir/*   $path
	echo "Done."
elif [ "$choice" ]
then
	echo "Please type a valid choice."
	exit
fi

pid_cogserver=$(lsof -ti :17001)
pid_relex=$(lsof -ti :4444)

if [ $pid_cogserver ]; then echo "Cogserver is already running.Kill cogserser."; kill -9 $pid_cogserver;  fi
if [ $pid_relex ]; then echo "Relex server is already running.Kill relex."; kill -9 $pid_relex;  fi

python $exp_dir/scripts/launch_exp.py 1 $oc_dir
