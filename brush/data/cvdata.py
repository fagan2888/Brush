from brush.data.data import Data
from sklearn.model_selection import train_test_split

class CVData:
    """A mostly Python class that manages training and validation data"""
    # def __init__(self, np.ndarray X, np.ndarray y, bool classification,
    #         np.ndarray X_val=None, np.ndarray y_val= None):
    # cdef Data train
    # cdef Data val
    # cdef bool classification, shuffle
    # cdef float split

    def __init__(self, X, y, classification,
                 shuffle, split, random_state=None):
        self.classification = classification
        self.shuffle = shuffle
        self.split = split

        if split < 1.0:
            self._split(X,y)

    def _split(self, X,y):
        stratify = y if self.classification else None
        X_train, X_val, y_train, y_val = train_test_split(X, y, 
                test_size=1-self.split, shuffle = self.shuffle,
                stratify = stratify)
        self.train = Data(X_train, y_train, self.classification)
        self.val = Data(X_val, y_val, self.classification)

    def get_batch(self, batch_size):
        """returns a new CVData object with batch sampled training data 
        and the same validation data as this CVData object"""
        newdata = self
        newdata.train = self.train.get_batch(batch_size)
        return newdata
