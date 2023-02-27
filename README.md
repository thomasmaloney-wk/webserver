# Web Server

A simple webserver project in C++.

## Building

All that's needed to build this project is running `make` in your terminal:

```
$ make
```

## Running
To run the server, you need to specify a port number.

```
$ ./server --port <PORT_NUMBER>
```

I added a convenience flag `--local` to default to 8080.
So for example, if you ran:

```
$ ./server --local
```

That would be equivalent to running:

```
$ ./server --port 8080
```

## Logging
By default, it writes all logs to standard out, however, I added a flag `--logfile` that allows you to specify a file for it to write the logs to instead.

For example:

```
$ ./server --local --logfile server.log
```

Would run the server on port 8080 and write all logs to `server.log`.

One thing I should note, is that it _kinda_ doesn't work at the moment.
The most that it's able to log to a file right now is just a single character `[`.
Currently trying to debug that and hopefully have it fixed soon, since it's kind of annoying.