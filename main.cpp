#include <iostream>
#include <stdexcept>


template <typename T = int>   // T stand in for any data type you choose, defaulting to int


// Generic Array class
class genericarray
{
    private:
	    T *data;  // pointer to the array stored on heap
	    int length;  // how many elements are currently stored

    public:
	    genericarray() : data(nullptr), length(0) {}   // starts empty, no memory yet
	    genericarray(const genericarray &other);  // copy constructor
	    ~genericarray(); // destructor
	    genericarray &operator=(const genericarray &rhs);

	    void addelement(T param);  // add an element to the end
	    T at(int param) const;  // get the value at an index
	    int size() const    // number of elements stored
	    {
	        return length;
            }

	    T sum() const;   // total of all elements
	    T max() const;   // largest element
	    T min() const;   // smallest element
	    T *slice(int begin, int end) const;   // new array holding begin, end
};



// copy construcotr
template <typename T>

genericarray<T>::genericarray(const genericarray &other) : data(nullptr), length(other.length)
{
    if(length>0)
    {
        data = new T[length];
	for(int i = 0; i < length; i++)
	{
	    data[i] = other.data[i];
	}
    }
}


// dstructor
template <typename T>

genericarray<T>::~genericarray()
{
    delete[] data;
}


// free old memory, then take copy of rhs
template <typename T>

genericarray<T> &genericarray<T>::operator=(const genericarray &rhs)
{
    if(this == &rhs)   // a=a, nothing to do
    {
        return *this;
    }

    delete[] data;   // free old memory
    length = rhs.length;
    data = nullptr;

    if(length>0)
    {
        data = new T[length];
	for(int i = 0; i < length; i++)
	{
	    data[i] = rhs.data[i];
	}
    }

    return *this;
}



// addelement: make a bigger array, copy old values, add new one, delete the old array
template <typename T>

void genericarray<T>::addelement(T param)
{
    T *new_data = new T[length + 1];   // new array, one bigger
    for(int i = 0; i < length; i++)
    {
        new_data[i] = data[i];   // copy the old values over
    }

    new_data[length] = param;   // the new value goes at the end

    delete[] data;  // delete the old array
    data = new_data;  // use the new array from now on
    length++;
}



// at: return the value at an index, or throw error if index is out of range
template <typename T>

T genericarray<T>::at(int param) const
{
    if(param<0 || param >= length)
    {
        throw std::out_of_range("genericarray::at(): index out of range");
    }

    return data[param];
}



// sum: add every element into running total
template <typename T>

T genericarray<T>::sum() const
{
    T total = T();
    for(int i = 0; i < length; i++)
    {
        total += data[i];
    }

    return total;
}



// max: start with the first element, replace it whenever we see a bigger one
template <typename T>

T genericarray<T>::max() const
{
    if(length==0)   // an empty array has no maximum
    {
        throw std::out_of_range("genericarray::max(): array is empty");
    }

    T result = data[0];

    for(int i=1; i<length; i++)
    {
        if(data[i] > result)
	{
	    result = data[i];
	}
    }

    return result;
}



// min: start with the first element, replace it whenever see smaller one
template <typename T>

T genericarray<T>::min() const
{
    if(length==0)  // an empty array has no min
    {
        throw std::out_of_range("genericarray::min(): array is empty");
    }
    
    T result = data[0];

    for(int i=1; i<length; i++)
    {
        if(data[i] < result)
        {
            result = data[i];
        }
    }   
    
    return result;
}



// slice: returns new array with elements from position begin to end
template <typename T>
T *genericarray<T>::slice(int begin, int end) const
{
    if (begin < 1 || end > length || begin > end)  // begin >= 1, end <= size, and begin must not pass end
    {
        std::cerr << "GenericArray::slice(): invalid bounds [" << begin << ", "
                  << end << "] for array of size " << length << "\n" << std::endl;
        return nullptr;
    }

    int n = end - begin + 1;  // how many elements in the slive
    T *result = new T[n];  // seperate copy, not the orginal data
    for (int i = 0; i < n; i++)
        result[i] = data[begin - 1 + i];     // positions start at 1, array index at 0
    return result;
}



int main()
{
    genericarray<int> int_array;  // build an int array of 12 numbers
    for(int i=0; i<12; i++)
    {
        int_array.addelement(i*3-5);
    }


    // print text size, sum, max, min, at
    std::cout << "\nsize of array is " << int_array.size() << "\n" << std::endl;
    std::cout << "sum of the array is " << int_array.sum() << "\n" << std::endl;
    std::cout << "maximum of array is " << int_array.max() << "\n" << std::endl;
    std::cout << "minimum of array is " << int_array.min() << "\n" << std::endl;
    std::cout << "at(3) = " << int_array.at(3) << "\n" << std::endl;


    // test slice with valid bounds
    int *sliced_array = int_array.slice(5, 10);
    if (sliced_array != nullptr)
    {
        int n = 10 - 5 + 1;   // number of elements in the slice
        std::cout << "slice(5,10) = ";
        for (int i = 0; i < n; i++)
        {
            std::cout << sliced_array[i] << " ";
	}
	std::cout << "\n" << std::endl;
        delete [] sliced_array;   // slice maade a new array, so we delete it
    }


    // test slice with bad bounds, print "invalid bounds"
    int *bad = int_array.slice(5, 100);       // end out of range -> nullptr
    std::cout << "bad slice returned " << (bad == nullptr ? "nullptr" : "array") << "\n" << std::endl;
    int *bad2 = int_array.slice(0, 3);        // positions start at 1 -> nullptr
    std::cout << "slice(0,3) returned " << (bad2 == nullptr ? "nullptr" : "array") << "\n" << std::endl;
    try   // test at() with a bad index on purpose, catch block handles the error
    {
        int_array.at(99);   // only indexes 0 to 11 exist
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "caught: " << e.what() << "\n" << std::endl;
    }


    genericarray<int> ex;
    for (int v = 0; v <= 8; v += 2)
    {
        ex.addelement(v);
    }
    int *s = ex.slice(2, 4);
    std::cout << "[0,2,4,6,8].slice(2,4) = ";
    for (int i = 0; i < 3; i++)
    {
        std::cout << s[i] << " ";
    }
    std::cout << "\n" << std::endl;
    delete [] s;

    // same class works with double because it is a template
    genericarray<double> d_array;
    d_array.addelement(1.5);
    d_array.addelement(-2.25);
    d_array.addelement(10.0);
    std::cout << "double: size=" << d_array.size() << "\n\n" << " sum=" << d_array.sum()  << "\n\n"
              << " max=" << d_array.max() << "\n\n" << " min=" << d_array.min() << "\n" << std::endl;

    // test copy constructor, changing the copy and must not change the original
    genericarray<double> copy(d_array);
    copy.addelement(100.0);
    genericarray<double> assigned;
    assigned = copy;
    std::cout << "copy size=" << copy.size() << "\n\n" << " assigned sum=" << assigned.sum() << "\n\n"
              << " original size=" << d_array.size() << "\n" << std::endl;

    return 0;
}



