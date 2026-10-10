#include <iostream>
using namespace std;

class Point
{
private:
    int x, y;

public:
    Point()
    {
        x = 0;
        y = 0;
    }

    Point(int a, int b)
    {
        x = a;
        y = b;
    }

    Point operator+(const Point& obj) const
    {
        return Point(x + obj.x, y + obj.y);
    }

    Point operator-(const Point& obj) const
    {
        return Point(x - obj.x, y - obj.y);
    }

    Point operator*(const Point& obj) const
    {
        return Point(x * obj.x, y * obj.y);
    }

    Point operator/(const Point& obj) const
    {
        if (obj.x == 0 || obj.y == 0)
            throw runtime_error("Division by zero");

        return Point(x / obj.x, y / obj.y);
    }

    bool operator<(const Point& obj) const
    {
        return x + y < obj.x + obj.y;
    }

    bool operator>(const Point& obj) const
    {
        return x + y > obj.x + obj.y;
    }

    Point& operator++()
    {
        ++x;
        ++y;
        return *this;
    }

    Point operator++(int)
    {
        Point temp = *this;
        ++(*this);
        return temp;
    }

    Point& operator--()
    {
        --x;
        --y;
        return *this;
    }

    Point operator--(int)
    {
        Point temp = *this;
        --(*this);
        return temp;
    }

    friend ostream& operator<<(ostream& os, const Point& obj)
    {
        os << "(" << obj.x << ", " << obj.y << ")";
        return os;
    }

    friend istream& operator>>(istream& is, Point& obj)
    {
        is >> obj.x >> obj.y;
        return is;
    }
};

template <class T>
class Matrix
{
private:
    T** elements;
    int rows;
    int columns;

    void Allocate(int r, int c)
    {
        rows = r;
        columns = c;

        elements = new T*[rows]{};

        try
        {
            for (int i = 0; i < rows; i++)
                elements[i] = new T[columns]{};
        }
        catch (...)
        {
            for (int i = 0; i < rows; i++)
                delete[] elements[i];

            delete[] elements;

            elements = nullptr;
            rows = 0;
            columns = 0;
            throw;
        }
    }

    void Clear()
    {
        for (int i = 0; i < rows; i++)
            delete[] elements[i];

        delete[] elements;

        elements = nullptr;
        rows = 0;
        columns = 0;
    }

public:
    Matrix()
    {
        elements = nullptr;
        rows = 0;
        columns = 0;
    }

    Matrix(int r, int c) : Matrix()
    {
        if (r <= 0 || c <= 0)
            throw invalid_argument("Invalid matrix size");

        Allocate(r, c);
    }

    Matrix(const Matrix& obj) : Matrix()
    {
        if (obj.rows == 0)
            return;

        Allocate(obj.rows, obj.columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                elements[i][j] = obj.elements[i][j];
        }
    }

    Matrix(Matrix&& obj)
    {
        elements = obj.elements;
        rows = obj.rows;
        columns = obj.columns;

        obj.elements = nullptr;
        obj.rows = 0;
        obj.columns = 0;
    }

    ~Matrix()
    {
        Clear();
    }

    Matrix& operator=(const Matrix& obj)
    {
        if (this != &obj)
        {
            Matrix temp(obj);
            *this = std::move(temp);
        }

        return *this;
    }

    Matrix& operator=(Matrix&& obj)
    {
        if (this != &obj)
        {
            Clear();

            elements = obj.elements;
            rows = obj.rows;
            columns = obj.columns;

            obj.elements = nullptr;
            obj.rows = 0;
            obj.columns = 0;
        }

        return *this;
    }

    void FillRandom()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                elements[i][j] = rand() % 9 + 1;
        }
    }

    void FillKeyboard()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                cout << "Element [" << i << "][" << j << "]: ";
                cin >> elements[i][j];
            }
        }
    }

    Matrix& operator++()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                ++elements[i][j];
        }

        return *this;
    }

    Matrix operator++(int)
    {
        Matrix temp(*this);
        ++(*this);
        return temp;
    }

    Matrix& operator--()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                --elements[i][j];
        }

        return *this;
    }

    Matrix operator--(int)
    {
        Matrix temp(*this);
        --(*this);
        return temp;
    }

    Matrix operator+(const Matrix& obj) const
    {
        if (rows != obj.rows || columns != obj.columns)
            throw invalid_argument("Different matrix sizes");

        Matrix result(rows, columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                result.elements[i][j] =
                    elements[i][j] + obj.elements[i][j];
        }

        return result;
    }

    Matrix operator-(const Matrix& obj) const
    {
        if (rows != obj.rows || columns != obj.columns)
            throw invalid_argument("Different matrix sizes");

        Matrix result(rows, columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
                result.elements[i][j] =
                    elements[i][j] - obj.elements[i][j];
        }

        return result;
    }

    Matrix operator*(const Matrix& obj) const
    {
        if (columns != obj.rows)
            throw invalid_argument("Cannot multiply matrices");

        Matrix result(rows, obj.columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < obj.columns; j++)
            {
                result.elements[i][j] = T{};

                for (int k = 0; k < columns; k++)
                {
                    result.elements[i][j] =
                        result.elements[i][j] +
                        elements[i][k] * obj.elements[k][j];
                }
            }
        }

        return result;
    }

    Matrix operator/(const Matrix& obj) const
    {
        if (rows != obj.rows || columns != obj.columns)
            throw invalid_argument("Different matrix sizes");

        Matrix result(rows, columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                result.elements[i][j] =
                    elements[i][j] / obj.elements[i][j];
            }
        }

        return result;
    };

    T FindMaximum() const
    {
        if (rows == 0 || columns == 0)
            throw runtime_error("Matrix is empty");

        T maximum = elements[0][0];

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (elements[i][j] > maximum)
                    maximum = elements[i][j];
            }
        }

        return maximum;
    }

    T FindMinimum() const
    {
        if (rows == 0 || columns == 0)
            throw runtime_error("Matrix is empty");

        T minimum = elements[0][0];

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (elements[i][j] < minimum)
                    minimum = elements[i][j];
            }
        }

        return minimum;
    }

    T& operator()(int r, int c)
    {
        if (r < 0 || r >= rows || c < 0 || c >= columns)
            throw out_of_range("Invalid index");

        return elements[r][c];
    }

    friend ostream& operator<<(ostream& os, const Matrix& obj)
    {
        for (int i = 0; i < obj.rows; i++)
        {
            for (int j = 0; j < obj.columns; j++)
                os << obj.elements[i][j] << "\t";

            os << endl;
        }

        return os;
    }


    friend istream& operator>>(istream& is, Matrix& obj)
    {
        for (int i = 0; i < obj.rows; i++)
        {
            for (int j = 0; j < obj.columns; j++)
            {
                is >> obj.elements[i][j];
            }
        }

        return is;
    }
};

template <>
void Matrix<Point>::FillRandom()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            elements[i][j] =
                Point(rand() % 9 + 1, rand() % 9 + 1);
        }
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "FIRST MATRIX" << endl;

    Matrix<Point> firstMatrix(2, 2);
    firstMatrix.FillRandom();

    cout << firstMatrix << endl;

    cout << "SECOND MATRIX" << endl;

    Matrix<Point> secondMatrix(2, 2);
    secondMatrix.FillRandom();

    cout << secondMatrix << endl;

    cout << "ADDITION" << endl;
    cout << firstMatrix + secondMatrix << endl;

    cout << "SUBTRACTION" << endl;
    cout << firstMatrix - secondMatrix << endl;

    cout << "MULTIPLICATION" << endl;
    cout << firstMatrix * secondMatrix << endl;

    cout << "DIVISION" << endl;
    cout << firstMatrix / secondMatrix << endl;

    cout << "MAXIMUM" << endl;
    cout << firstMatrix.FindMaximum() << endl;

    cout << "MINIMUM" << endl;
    cout << firstMatrix.FindMinimum() << endl;

    cout << "INCREMENT" << endl;
    firstMatrix++;
    cout << firstMatrix << endl;

    cout << "DECREMENT" << endl;
    --firstMatrix;
    cout << firstMatrix << endl;

    cout << "KEYBOARD INPUT" << endl;

    Matrix<Point> keyboardMatrix(2, 2);

    cout << "Enter x and y for each point:" << endl;
    keyboardMatrix.FillKeyboard();

    cout << "\nYour matrix:" << endl;
    cout << keyboardMatrix;

}
