bruco_proxy
================
bruco_proxy is simple intrusion prevention proxy.


requirements
================
<pre>
  $ wget http://re2.googlecode.com/files/re2-20130622.tgz
  $ tar xvfz re2-20130622.tgz
  $ cd re2
  $ make
  $ make install
</pre>


build
================
<pre>
$ git clone https://github.com/yoggy/bruco_proxy.git
$ cd bruco_proxy
$ cmake .
$ make
</pre>

run
================
<pre>
$ ./bruco_proxy
usage : ./bruco_proxy config_file

    examples
        $ ./bruco_proxy sample_config.txt

$ ./bruco_proxy sample_config.txt
==== startup bruco_proxy ====
2013/06/16 20:01:53 [D] config_file=sample_config.txt, listen_port=12345, forward_host=::1, forward_port=80, max_client=100
2013/06/16 20:01:53 [I] listen start...port=12345
</pre>

config file
================
<pre>
# log level (0:debug, 1:info, 2:warn)
output_log_level = 0

# max output string length
output_string_length = 1024

# network config
listen_port = 12345
forward_host  = ::1
forward_port  =A23456 
max_client  = 100

# dump stream (true / false)
dump_stream = true

# key check in outbound connection (true / false)
outbound_key_check        = true
outbound_key_check_xor256 = false
outbound_key_file         = ./key.txt

# inbound jmpcall detect
inbound_jmpcall_check = false

# regular expression check
inbound_deny  = \x61\x61\x61\x61
outbound_deny = \x62\x62\x62\x62

</pre>

