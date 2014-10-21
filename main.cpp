#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char INPUT_FILE[] = "input/dataset.txt";

void InsSort(int num[ ], int n)
{
    int i, j, temp;

    for (i = 1; i < n; i++) {
        temp = num[i];
        for (j = i; j > 0 && num[j-1] > temp; j--)
            num[j] = num[j -1];

        num[j] = temp;
    }
}

int readDatasetFile(double x[100], double y[100], double x_[100], double y_[100])
{
    FILE *fp;
    int ret;
    int id, cnt = 0;

    if ((fp = fopen(INPUT_FILE, "r")) == NULL){
        printf("File Open Error\n");
        return -1;
    }

    while ((ret = fscanf(fp, "%d %lf %lf %lf %lf", &id, &x[cnt], &y[cnt], &x_[cnt], &y_[cnt])) != EOF){
        cnt++;
    }
    return cnt;
}

int main(int argc,char *argv[])
{
    int numPoint;
    //Array of x y x' y'
    double x[100], y[100], x_[100], y_[100];

    //Read x y x' y' from dateset.txt
    numPoint = readDatasetFile(x, y, x_,y_);

    //Please write your code here.
    //

    // definision
    // Paras of Affine Trans.
    double a[100][100];
    double b[100][100];
    double c[100][100];
    double d[100][100];

    // array which contains translation error
    double err[100][100][100];

    // determinant of Affine mat.
    double det  = 0.;

    // threshold
    double th   = 1.;

    // num of outliers
    int counter[100][100];

    // i, j corresponding to index 1, 2
    // execute RANSAC using all of pairs (e.g. 0and0, 0and1, 0and2, ... ,99and99)
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){

            // determinant (required in solving inv matrix of x, y)
            det = x[i] * y[j] - x[j] + y[i];

            // paras of affine matrix (a to d)
            a[i][j] = (+ x_[i] * y[j] - y[i] * x_[j]) / det;
            b[i][j] = (- x_[i] * x[j] + x[i] * x_[j]) / det;
            c[i][j] = (+ y_[i] * y[j] - y[i] * y_[j]) / det;
            d[i][j] = (- y_[i] * x[j] + x[i] * y_[j]) / det;

            // compute error using solved affine mat in id=k point.
            for(int k=0; k<100; k++){
                err[i][j][k] = sqrt(pow((x_[k] - (a[i][j] * x[k] + b[i][j] * y[k])), 2) + pow((y_[k] - (c[i][j] * x[k] + d[i][j] * y[k])), 2));

                // count num of error over the threshold
                if (err[i][j][k] > th){
                    counter[i][j]++;
                }
            }
        }
    }

    // contain suitable pair using in Ransac
    int i_m, j_m;

    double tmp = 100000000.0;

    // find suitable pair
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
            double sum = 0;
            for(int k=0; k<100; k++){
                sum += err[i][j][k];
            }
            // tmp contains minimum sum_k(err[i][j][k]) and i_m, j_m contains i, j in minimum sum_k(err[i][j][k])
            if(sum < tmp){
                tmp = sum;
                i_m = i;
                j_m = j;
            }
        }
    }
    // display all results
    printf("Most suitable pair of points used in RANSAC:\n > (%d, %d) \n", i_m, j_m);
    printf("IDs of outliers: \n > ");
    int cnt = 0;
    for(int k=0; k<100; k++){
        if (err[i_m][j_m][k] > th){
            printf("%d, ",  k);
            cnt++;
        }
    }
    printf("\nNum of outliers: \n");
    printf(" > %d\n", cnt);
    printf("Params of AFIINE Trans. :\n > a = %3.1f, b = %3.1f, \n > c = %3.1f, d = %3.1f\n", a[i_m][j_m], b[i_m][j_m], c[i_m][j_m], d[i_m][j_m]);
    return 0;
}
