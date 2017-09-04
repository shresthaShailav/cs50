import cs50

def main():
    # get card number and length
    print ("Number : ", end = "")
    while True:
        card = cs50.get_int()
        if (card > 0):
            break
    length = len(str(card))
    
    #check for length validity
    if (length != 13 and length != 15 and length != 16):
        print("INVALID")
        exit(0)
    
    #get initial two digits
    initial = int(str(card)[:2])
    
    #check with Luhn's algorithm
    sum_n = 0
    even_check = 1
    while card!= 0:
        x = card % 10
        
        if even_check % 2 == 0 :
            z = x * 2
            while z != 0 :
                sum_n += z % 10
                z = z // 10
        else :
            sum_n += x
    
        card = card // 10
        even_check += 1
    
    #display card type if valid
    if (sum_n % 10 == 0) :
        if ((initial == 34 or initial == 37) and length == 15):
            print("AMEX")
        elif ((initial > 50 and initial < 56) and length == 16):
            print("MASTERCARD")
        elif ((initial // 10 == 4) and (length == 13 or length == 16)):
            print("VISA")
        else:
            print("INVALID")
    else :
        print("INVALID")

    
if __name__ == "__main__":
    main()
    