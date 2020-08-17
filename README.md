### Official website

The official website for PANDA can be found here:
http://comopt.ifi.uni-heidelberg.de/software/PANDA/

### Note

This is an unofficial repository, although by the original author, solely for the purpose of having an archive of the software.
If you found any bug, please raise an issue.

### License

Creative Commons Attribution-NonCommercial 4.0 International License
http://creativecommons.org/licenses/by-nc/4.0/

### Docker

For convenience there is a docker image available.

```
docker run -i --rm -v"$(pwd)":/data panda panda /data/samples/panda_format/sample_1
// does the same as panda samples/panda_format/sample_1
// if you had installed panda manually
```

Explanation of the above command:

```
docker run -i --rm -v"$(pwd)":/data panda panda /data/samples/panda_format/sample_1
           ^  ^    ^                ^     ^     ^
           |  |    |                |     |     any panda command line argument goes here
           |  |    |                |     |
           |  |    |                |     run the panda command
           |  |    |                |
           |  |    |                use the panda image
           |  |    |
           |  |    map the current directory, such that files become available to the container
           |  |
           |  delete container when done
           |
           run in interactive mode such that output will be printed to terminal
```

#### Using the panda docker image on [hub.docker.com](https://hub.docker.com/r/pandacomopt/panda)

```
docker pull pandacomopt/panda
docker tag pandacomopt/panda panda
```

#### Building the docker image manually:

```
docker build -t panda .
```
