
## How to generate a new SSH key for your GitHub.com account
> Whenever you create a new Ubuntu (or Linux) instance there is a .ssh directory that keeps track of zero or more SSH keys that are used by sites and servers to allow access over ssh. GitHub.com is a classic example of this. In order to allow SSH access to that site you must generate a SSH key and add it to your GitHub.com account.

 1. **GIVEN** we need access GitHub.com using ssh 
 2. **WHEN** we create a unique ssh key for the Ubuntu instance
 3. **THEN** we can access our GitHub.com account using the git utility (better)

### Prerequisites
 1. Github.com account (or something similiar)

### Resources
 1. [Generating a new SSH key and adding it to the ssh-agent](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)

### Wish Case
Assuming you have a GitHub.com account:
 1. Open a Terminal box:

		ssh-keygen -t ed25519 -C "your_email@example.com"

2. Where you supply the same email address used for your GitHub.com account (instead of "your_email@example.com") and do not supply a pass phrase unless you want a popup to show up all the time asking it (sometimes you want this extra layer of security but generally it is not required).
3. Now show the contents of your public key:

		cat ~/.ssh/id_ed25519.pub

4. Copy and paste the contents of that file into your github account under **User->Settings->SSH & GPG** keys
5. You should be good to go on any git clone operations needing a SSH key for your current Ubuntu instance

### Alternate Case
> **user.email not specified**<br/>
> **user.name not specified**<br/>
> You must specify your name and email for the git client to work<br/>
>  `git config --global user.name "My Name"`<br/>
> `git config --global user.email "myemail@example.com"`

### Summary
> This should setup your SSH keys for your command line git client access to your GitHub.com account

### Next Steps
- [How to clone your project (with this template)](https://github.com/perriera/extras_oci/blob/dev/docs/CLONE.md)



