# Passgen

Description
---------
Passgen is a password generator. It can be used to create fully randomed passwords or used with a wordfile
so that the password can be remembered more easily.


Version
-------
0.1

How to use
----------
```sh
passgen [-n number] [-w wordFile] [-s specialFile] [-l length] [-ls specials] [-ln numbers] [-lc capitals]
```

```
n number
```


```sh
  n number : number of passwords to generate
  w wordFile :  path to a text file containing a word per line
  s specialFile : path to a text file containing a special character per line
  l length : length of every passwords to generate
  ls specials : number of specials to use
  ln numbers : number of specials to use
  lc capitals : number of capitals to use
```
By default, it generates a 16 character long password with only lower case letters
