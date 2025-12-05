#include "stdafx.h"
#include "Matrix.h"

Matrix::Matrix(int row, int col): rows(row), cols(col), data(row, std::vector<double>(col, 0.0))
{
	
}

Matrix::~Matrix(){} 

double& Matrix::operator()(int row, int col) { return data[row][col]; }

Matrix Matrix::getIdentity()
{
	Matrix identity(4, 4);
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			if (i == j)
				identity(i, j) = 1.0;
			else
				identity(i, j) = 0.0;
		}
	}
	return identity;
}

Matrix Matrix::operator+(const Matrix& other)
{
	Matrix result(rows, cols);
	if (this->rows == other.rows && this->cols == other.cols)
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
	Matrix result(this->rows, other.cols);

	for (int i = 0; i < this->rows; i++)
	{
		for (int j = 0; j < other.cols; j++)
		{
			result.data[i][j] = 0;
			for (int k = 0; k < this->cols; k++) // this->cols == other.rows
			{
				result.data[i][j] = result.data[i][j] + (data[i][k] * other.data[k][j]);
			}
		}
	}

	return result;
}




Matrix Matrix::getTranslationMatrix(double tx, double ty, double tz)
{
	Matrix transMat = getIdentity();
	transMat(0, 3) = tx;
	transMat(1, 3) = ty;
	transMat(2, 3) = tz;
	return transMat;
}

Matrix Matrix::getScalingMatrix(double sx, double sy, double sz)
{
	Matrix scaleMat = getIdentity();
	scaleMat(0, 0) = sx;
	scaleMat(1, 1) = sy;
	scaleMat(2, 2) = sz;
	return scaleMat;
}

Matrix Matrix::getRotationXMatrix(double degreeX)
{
	Matrix rotXMat = getIdentity();
	double radAngX = degreeX * MathConstants::PI / 180.0;
	double cosX = cos(radAngX);
	double sinX = sin(radAngX);

	rotXMat(1, 1) = cosX;  rotXMat(1, 2) = -sinX;
	rotXMat(2, 1) = sinX;  rotXMat(2, 2) = cosX;
	return rotXMat;
}

Matrix Matrix::getRotationYMatrix(double degreeY)
{
	Matrix rotYMat = getIdentity();
	double radAngY = degreeY * MathConstants::PI / 180.0;
	double cosY = cos(radAngY);
	double sinY = sin(radAngY);

	rotYMat(0, 0) = cosY;  rotYMat(0, 2) = sinY;
	rotYMat(2, 0) = -sinY;  rotYMat(2, 2) = cosY;
	return rotYMat;
}

Matrix Matrix::getRotationZMatrix(double degreeZ)
{
	Matrix rotZMat = getIdentity();
	double radAngZ = degreeZ * MathConstants::PI / 180.0;
	double cosZ = cos(radAngZ);
	double sinZ = sin(radAngZ);

	rotZMat(0, 0) = cosZ;  rotZMat(0, 1) = -sinZ;
	rotZMat(1, 0) = sinZ;  rotZMat(1, 1) = cosZ;
	return rotZMat;
}