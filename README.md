# trivial-file-appender
A simple simple toy lib provides 3 api to append info into file with simple CRC check, while you can just open the file as nothing happened.
Not responsible for users' behavior.

# Info format

<pre>
+---------+------------+---------+---------+  
|  CRC32  |  content   |  size   |  sign   |  
+---------+------------+---------+---------+  
| 4 bytes | $size bytes| 4 bytes | 8 bytes |  
+---------+------------+---------+---------+  
</pre>

# Build

clone repo and its submodule, then use cmake 

# How to use

[API doc](https://github.com/sora-blue/trivial-file-appender/blob/master/appender.h)

# Why this?
It is designed as a mild, universal & light way to claim ownership rather than filename at first. When
illustrators post their images, i appreciate their work and want to obey their rules one by one
such as not using it as profile image, etc. I'm tired of copying and pasting the intro on their
profile one by one so i'm thinking of a tampermonkey script to automate the process, during which
these apis write their profile into file, in concept, as it may not work with javascript or needs to
be external.
