import sys
import cs50
import crypt

MAX = 4
salt = ""
password = ""

def main():
    
    # ensure proper usage
    if len(sys.argv) != 2 :
        print("Error! Usage crack.py <hash>")
        exit(1)
        
    # get hash and prepare for check
    hash_pass = sys.argv[1]
    global salt
    salt = hash_pass[:2]
    
    # prints word if found
    if (check(MAX, hash_pass) == True):
        print(password)
    else:
        print("The password is not an alphabet of length {}".format(MAX))
        
        
    
""" 
    returs true if any of the combination of words (between 1 to num_char characters) is the password 
    returns false if password not found
"""
def check(num_char, hash_pass):
    for i in range(1, num_char + 1):
        word = ""
        if(check_word(word, i, hash_pass) == True):
            return True
    return False
    
    
    
    
"""
    permutates num_char letters from a to z and checks if the crypted result matches the hash
    returns true if matched
    returns false if not matched
"""
    
def check_word(word, max_char, hash_pass):
    if max_char < 1:
        return False
    if max_char == 1:
        for i in range(ord('A'), ord('z') + 1):
            final_word = word + chr(i)
            
            # check if final word is the password
                # if yes return true
            #if ()
            #if (hash_pass == "catoy"):
            #    return True
                
            if (crypt.crypt(final_word, salt) == hash_pass):
                global password
                password = final_word
                return True
                
                
            final_word = ""
    else:
        for j in range(ord('A'), ord('z') + 1):
            n_final_word = word + chr(j)
            if (check_word(n_final_word, max_char - 1, hash_pass) == True):
                return True
            n_final_word = ""
    return False
            
    
if __name__ == "__main__" :
    main()
    