# Find the cat

## Contributors
- Albert BECQUET
- Lucas RIOUX

## Run the project
Execute the command `make clean && make main` to create the executable.
Then, run `./ftc starting-point [-option [parameter]]`.

## Options
- `-name` : search by name.
- `-date` : search by date.
- `-mime` : search by MIME type.
- `-ctc` : search using a regex inside the files.
- `-dir` : search by directory.
- `-perm` : search by permissions.
- `-color` : colorize file names according to their MIME type.
- `-ou` (french for OR) : by default, files must match all specified condtions. With this option, files must match at least one condition.

If you make a mistake, the program will tell you what went wrong. Also, you can refer to the project instructions if you need further clarification.
