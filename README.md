
Build project with script:
```
 env ESP_BUILD_DIR=<build-dir> ./tools/scripts/build-es-player
```
> Note: default value for \<build-dir\> is build-out/

Run server:
```
 <build-dir>/esplayerd/esplayerd
```

Run client:
```
 <build-dir>/libesplayer/libesplayer
```

For GStreamer debug set:
```
GST_DEBUG=<level>
```
| level | Name | Description |
| :---: | :--- | :--- |
| 0 | none    | No debug information is output. |
| 1 | ERROR   | Logs all fatal errors. These are errors that do not allow the core or elements to perform the requested action. The application can still recover if programmed to handle the conditions that triggered the error. |
| 2 | WARNING | Logs all warnings. Typically these are non-fatal, but user-visible problems are expected to happen. |
| 3 | FIXME   | Logs all "fixme" messages. Those typically that a codepath that is known to be incomplete has been triggered. It may work in most cases, but may cause problems in specific instances. |
| 4 | INFO    | Logs all informational messages. These are typically used for events in the system that only happen once, or are important and rare enough to be logged at this level. |
| 5 | DEBUG   | Logs all debug messages. These are general debug messages for events that happen only a limited number of times during an object's lifetime; these include setup, teardown, change of parameters, etc. |
| 6 | LOG     | Logs all log messages. These are messages for events that happen repeatedly during an object's lifetime; these include streaming and steady-state conditions. This is used for log messages that happen on every buffer in an element for example. |
| 7 | TRACE   | Logs all trace messages. Those are message that happen very very often. This is for example is each time the reference count of a GstMiniObject, such as a GstBuffer or GstEvent, is modified |
| 9 | MEMDUMP | Logs all memory dump messages. This is the heaviest logging and may include dumping the content of blocks of memory. |



To create Docker image with development env:
```
 ./tools/docker/build-docker-image --tag=<img-name>:<some-tag>
```

For GStreamer debug set:
```
GST_DEBUG=<level>
```
| level | Name | Description |
| :---: | :--- | :--- |
| 0 | none    | No debug information is output. |
| 1 | ERROR   | Logs all fatal errors. These are errors that do not allow the core or elements to perform the requested action. The application can still recover if programmed to handle the conditions that triggered the error. |
| 2 | WARNING | Logs all warnings. Typically these are non-fatal, but user-visible problems are expected to happen. |
| 3 | FIXME   | Logs all "fixme" messages. Those typically that a codepath that is known to be incomplete has been triggered. It may work in most cases, but may cause problems in specific instances. |
| 4 | INFO    | Logs all informational messages. These are typically used for events in the system that only happen once, or are important and rare enough to be logged at this level. |
| 5 | DEBUG   | Logs all debug messages. These are general debug messages for events that happen only a limited number of times during an object's lifetime; these include setup, teardown, change of parameters, etc. |
| 6 | LOG     | Logs all log messages. These are messages for events that happen repeatedly during an object's lifetime; these include streaming and steady-state conditions. This is used for log messages that happen on every buffer in an element for example. |
| 7 | TRACE   | Logs all trace messages. Those are message that happen very very often. This is for example is each time the reference count of a GstMiniObject, such as a GstBuffer or GstEvent, is modified |
| 9 | MEMDUMP | Logs all memory dump messages. This is the heaviest logging and may include dumping the content of blocks of memory. |

To execute created image:
```
 docker run -it --rm --hostname build <img-name>:<some-tag>
```
