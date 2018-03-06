# πBreeze
## LightWeight FileManager based on Qt5 

Forked from NewBreeze (v3-beta2), this is built as a dedicated file manager for r-pi.

Built on top of the Powerful Qt GUI Toolkit, this file manager is sleek, stylish and fast.
Some of its features are

* Fast startup and browsing
* File Association support
* Provides Categorized Icon View
* Sleek side panel showing drives and boomarks
* Full drag and drop support
* Catalog View and SuperStart
* Auto update devices list
* Custom Folder Icons
* Complete mime-icon support
* BreadCrumbs Navigation Bar
* Custom Actions and inbuilt terminal support
* Archive read write support via Plugins

## Linux Source Available
### Windows source is no longer being maintained

### Please READ the ReleaseNotes for information on the new features

### Notes for compiling - linux:

* Download the sources
   - Git: `git clone https://github.com/marcusbritanicus/NewBreeze NewBreeze-master`
   - Tar: `wget https://github.com/marcusbritanicus/NewBreeze/archive/master.tar.gz && tar -xf master.tar.gz`
* Enter `NewBreeze-master`
* Open the terminal and type: `qmake && make`
* To install, as root type: `make install`

### Dependencies:
* Qt5 (qtbase5-dev, qtbase5-dev-tools)
* xdg-utils (xdg-utils)
* EncFS (encfs)

### Known Bugs
* Extended copy/move causes crashes. Disabled by default.

### Upcoming
* Any other feature you request for... :)
