import cs50

def main():
    height = int_btwn(0,23)
    
    # some preparations
    x = height - 1
    
    # prints pyramind line by line
    for i in range(1, height + 1):
        
        print_hash(x, i)
        print_hash(2,i)
        print("")
        x -= 1
      
# returns integer between min and max inclusive  
def int_btwn(min, max):
    while True :
        print("Height : ", end = "")
        num = cs50.get_int()
        if (num >= min and num <= max): 
            return num
    
# prints blank space and hash given number of times
def print_hash(num_space, num_hash) :
    for i in range(num_space):
        print(" ", end = "")
    for j in range(num_hash):
        print("#", end = "")

if __name__ == "__main__" :
    main()
