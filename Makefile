all:
	echo $$PWD	
	for d in */*/;do echo $$d; (cd $$d && make); done

clean:
	for d in */*/;do (cd $$d && make clean); done
