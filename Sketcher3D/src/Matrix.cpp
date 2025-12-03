#include "stdafx.h"
#include "Matrix.h"

Matrix::Matrix(int row, int col): rows(row), cols(col){}

Matrix::~Matrix(){} 

double& Matrix::operator()(int row, int col) { return data[row][col]; }

Matrix Matrix::getIdentity()
{
	Matrix identity(4, 4);
	for (int i = 0; i < 4; i++)
		identity(i, i) = 1.0;
	return identity;
}

Matrix Matrix::operator+(const Matrix& other)
{
	Matrix result(rows, cols);
	if ((this->cols == other.rows))
	{

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				result.data[i][j] = this->data[i][j] + other.data[i][j];
			}
		}
		return result;
	}
	else
	{
		QMessageBox::information(nullptr, "Info", "Incorrect rows and columns");
	}
}

Matrix Matrix::operator*(const Matrix& other)
{
	Matrix result(this->cols, other.rows);

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < other.cols; j++)
		{
			result.data[i][j] = 0;
			for (int k = 0; k < this->cols; k++) // this->cols == other.rows
			{
				result.data[i][j] = result.data[i][j] + (data[i][k] * data[k][j]);
			}
		}
	}

	return result;
}



