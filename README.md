# trivial-file-appender
A simple simple toy lib provides 3 api to append info into file with simple CRC check, while you can just open the file as nothing happened.
Not responsible for users' behavior.

# Info format

<pre>
+----------+----------+----------+----------+  
|   CRC32  |  content |   size   |   sign   |  
+----------+----------+----------+----------+  
|  4 bytes |$size byte|  4 bytes |  8 bytes |  
+----------+----------+----------+----------+  
</pre>

# Build

clone repo and its submodule, then use cmake 

# How to use

[API doc](https://github.com/sora-blue/trivial-file-appender/blob/master/appender.h)
