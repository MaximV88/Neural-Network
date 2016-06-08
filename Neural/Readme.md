#Neural

Neural is a basic neural network that is operated via the command line, which is great for SSH!
<br>
Made with C++11. 

####Install

Run 'make' command in the folder directory.

Usage:<br>
-i  Specifies the input data file that has the raw data as 784 pixels per each read. <br>
-k  Specifies the key file that holds the answers for the given data file. <br>
-o  Specifies the name of the output file. <br>
-n  Specifies the type of network to use: 1 stands for 10 different networks, 2 will run with a single network. <br>
-t  Activates the test mode. The flag specifies name of the output file from a previous run. The -i file will be used as the test file and will output the results to the -o file.