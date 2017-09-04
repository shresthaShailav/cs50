from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # file objects for positive and negatives
        pos_file = open(positives, "r")
        neg_file = open(negatives, "r")
        
        # create and add to positive and negative sets
        self.positive_set = set()
        self.negative_set = set()
        
        for line in pos_file :
            # ignore comments
            if ";" in line:
                continue
            self.positive_set.add(line.strip("\n"))
        for line in neg_file :
            
            # ignore comments
            if ";" in line:
                continue
            self.negative_set.add(line.strip("\n"))
            
        # instantiate the TweetTokenizer
        self.Tokenizer = TweetTokenizer()
        

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
    
        word_list = self.Tokenizer.tokenize(text)
            
        # analyze and return  
        counter = 0
        for word in word_list :
            if word.lower() in self.positive_set :
                counter += 1
                
            if word.lower() in self.negative_set :
                counter -= 1
            
        return counter
