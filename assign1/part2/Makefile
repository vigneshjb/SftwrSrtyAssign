all:
	chmod 777 discovery
	javac Discover.java
vignesh:
	awk -F ',' '{print $$1}' ~/.ssh/known_hosts>>new.txt
	awk -F '\\t' '{ if($$2) print $$2}' /etc/hosts>>new.txt
