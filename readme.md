## Project setup

### antlr4 install on ubuntu or wsl
```
sudo apt update
sudo apt upgrade
sudo apt install cmake default-jdk unzip build-essential
mkdir ~/antlr4-install
cd ~/antlr4-install
wget https://www.antlr.org/download/antlr-4.13.2-complete.jar
wget https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip
unzip antlr4-cpp-runtime-4.13.2-source.zip
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=~/antlr4-install/ ..
make -j8
make  install
```

### Personal makefile configuration

```
cat <<EOF > compiler/config.mk
ANTLRJAR=/home/$USER/antlr4-install/antlr-4.13.2-complete.jar
ANTLRINC=/home/$USER/antlr4-install/include/antlr4-runtime/
ANTLRLIB=/home/$USER/antlr4-install/lib/libantlr4-runtime.a
EOF
```

### Execute tests
```
make test
```
To verbose :
```
make testv
```

### Formatter le code

ctrl + maj + i (Anatole)
maj + alt + f (les autres)

### CI : Pre-commit

In order to make sure code always well-formatted and commit messages respect standards, we will use `pre-commit`.

__Installation __: 
```
# 1. Install tool
pip install pre-commit 

# 2. Install hooks (code formatting)
pre-commit install 

# 3. Install hook for commit messages formatting 
pre-commit --hook-type commit-msg

# 4. (optional) Launch manual formatting of all files 
pre-commit run --all-files
```

```
# bypassing pre-commit check when committing
git commit -m <commit_msg> --no-verify
```