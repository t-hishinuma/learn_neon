TAG=0.0.1
NAME=neon_test

.PHONY: container in

all:

container: Dockerfile
	docker build -t $(NAME):$(TAG) .

in: Dockerfile
	docker run \
		-v $(PWD):/work \
		-w /work \
		-it --rm $(NAME):$(TAG)
