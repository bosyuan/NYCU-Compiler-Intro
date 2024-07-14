# hw1 report

|Field|Value|
|-:|:-|
|Name|侯博軒|
|ID|110550167|

## How much time did you spend on this project

8 hours 

    - 4 hours spent on reading and writing the code
    - 3.5 hours on debug
    - 0.5 hours on report

## Project overview

I divide tokens into 2 different kinds. First, simple tokens that could be hard coded into the scanner e.g. (, ), +, if, begin...etc. Second, tokens that have to be described by regular expressions. 
Simple tokens are recognized by hard coded patterns, output by calling the pre-defined function LIST_TOKEN(). For harder tokens described by regex, I define the regex at the definition part of scanner.l. Some of these tokens require more complicated process than simply calling LIST_TOKEN() function, for example comment and string tokens, I solve these issues by calling my own string managing C code when these tokens are detected. 
  
Overall, the scanner is capable of detecting all the pre-defined tokens written in given spec. This robust scanner will output various tokens according to input text format and return error if it is an invalid syntax. 
We preovide comment options to manipulate the output of our scanner, "//&S" "//&T" can be used to control the showign of tokens and input source text.  


## What is the hardest you think in this project

I spent some time figuring out how to output source text when my input is a comment token. I figured out that I had to write my own LIST_SOURCE function when the comment includes new lines. Because I'm not quite familiar with the C language (been using c++ since I passed Introduction to Computers...), it took quite a while for me to figure out how to write a code that could output the correct string from yytext.

## Feedback to T.A.s

Great HW! I enjoyed most of my time solving this homework. Learned a lot from implementing my own scanner. 
