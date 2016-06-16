# FridgeAlarm
## System overview
Alarm system for unintentionally open fridge door

## Requirements

## Design
### Component selection


### Box design

### PCB

### Software

#### Doxygen deployment by Travis
The source code documentation is generated with [Doxygen](www.doxygen.com Doxygen: Main Page) and pushed automatically to GitHub Pages by [Travis CI](www.travis-ci.org).

##### Setup
This assumes that you've already figures out how to genereate Doxygen documentation and are able to build with Travis CI.

To be able to automatically update Doxygen documentation when a commit is done to GitHub follow these steps:
###### Generation of SSH-key
1. Generate a new SSH key with the following command: 
`ssh-keygen -t rsa -b 4096 -C "your_email@example.com"`
2. You are then prompted to enter a name for the new keypair. Enter a file in which to save the key.
3. When asked for passphrase just press ENTER for empty passphrase. Confirm with ENTER again.

###### Setup GitHub
1. Navigate to your GitHub settings page `https://github.com/<username>/<your_repository>/settings/keys` and paste the content from the public ssh-key file, the one ending with .pub. **Make sure to check the box that allows write access!**

###### Setup Travis CI
This is the trickiest part.
1. Make sure you're able to connect to Travis console.
2. Run command: `travis encrypt-file name-of-key-generated`.
It's the key that doesn't end with .pub that shall be used.
3. You get an instruction from Travis CI. As it says you should now make sure to add the returned key, ending with .enc, to your repository. All others keys can and should now be removed!
4. Next update your .travis.yml with the encryption label, the 12-digit hex value returned from step 2.
```
env:
	global:
    	- ENCRYPTION_LABEL: "12-digit hex value"
```
TODO: Finalize
https://gist.github.com/domenic/ec8b0fc8ab45f39403dd





## Verification

### Power consumption
The following current consumption has been observed with prototype pcb, Attiny85 and magnetic buzzer.
System mode      |Current consumption [mA]
-----------------|:-----------------------:
Buzzer and LED ON|42
LED ON			 |2.3
Deep sleep		 |0.024

## Conclusion





