all: run

update:
	@rm ./library/library.json
	@python3 ./server.py

run:
	@python3 ./server.py

create_user:
	sudo useradd -m server

clean:
	@echo "Clean project"
	@rm -f *.json 
	@rm -r ./__pycache__

delete_user:
	sudo userdel -r server

