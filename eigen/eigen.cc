/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 310447 2011-04-23 21:14:10Z bjori $ */

extern "C"{
	#ifdef HAVE_CONFIG_H
	#include "config.h"
	#endif

	#include "php.h"
	#include "php_ini.h"
	#include "ext/standard/info.h"
	#include "php_eigen.h"
}

#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/SVD>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

// define
VectorXf read_vector(zval *vector,int type);
MatrixXf read_matrix(zval *matrix,int type);
zval* make_return_vector(VectorXf V);
zval* make_return_matrix(MatrixXf V);

/* If you declare any globals in php_eigen.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(eigen)
*/

/* True global resources - no need for thread safety here */
static int le_eigen;

/* {{{ eigen_functions[]
 *
 * Every user visible function must have an entry in eigen_functions[].
 */
const zend_function_entry eigen_functions[] = {
	PHP_FE(confirm_eigen_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(eigenworld, NULL)
	PHP_FE(Eigen2, NULL)
	PHP_ME(Eigen2,Singular_Value_Decomposition, NULL,0)
	PHP_ME(Eigen2,Inner_Product, NULL,0)
	PHP_FE(Eigen_Singular_Value_Decomposition, NULL)
	PHP_FE(Eigen_Feature_Vector , NULL)
	PHP_FE(Eigen_Inner_Product, NULL)
	PHP_FE(Eigen_Vector_Length, NULL)
	PHP_FE(Eigen_COS, NULL)
	PHP_FE_END	/* Must be the last line in eigen_functions[] */
};
/* }}} */

/* {{{ eigen_module_entry
 */
zend_module_entry eigen_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"eigen",
	eigen_functions,
	PHP_MINIT(eigen),
	PHP_MSHUTDOWN(eigen),
	PHP_RINIT(eigen),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(eigen),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(eigen),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EIGEN
ZEND_GET_MODULE(eigen)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("eigen.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_eigen_globals, eigen_globals)
    STD_PHP_INI_ENTRY("eigen.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_eigen_globals, eigen_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_eigen_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_eigen_init_globals(zend_eigen_globals *eigen_globals)
{
	eigen_globals->global_value = 0;
	eigen_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(eigen)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Eigen2", eigen_functions);
	eigen_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_string(eigen_ce, "interface",strlen("interface"), "eth2", ZEND_ACC_PUBLIC);


	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(eigen)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(eigen)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(eigen)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(eigen)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "eigen support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_eigen_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_eigen_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "eigen", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
PHP_FUNCTION(eigenworld)
{
	Eigen::MatrixXf X(2,2);
	X(0,0)=5.0;
	X(1,0)=5.0;
	X(0,1)=5.0;
	X(1,1)=1.0;
	X(1,1)=X(1,0) + X(0,1);
	std::cout << X << std::endl;	



	printf("eigenworld\n");
	return;
}
PHP_FUNCTION(Eigen2)
{
	printf("Eigen\n");
	return;
}
PHP_METHOD(Eigen2, Singular_Value_Decomposition)
{
	printf("Eigen_SVD\n");
	return;
}
PHP_METHOD(Eigen2, Inner_Product)
{
	printf("Eigen_IP\n");
	return;
}
PHP_FUNCTION(Eigen_Singular_Value_Decomposition)
{
	// この関数でやってること
	//-------- 配列要素の受け取り
	//-------- Eigen行列に値を格納する 
	//-------- 特異値分解を実施.　 
	//-------- 累積値が指定を満たすように計算し、潜在意味空間を作成する.
	//-------- 返却するzvalデータを作成する.
	//-------- 返却するオブジェクトを作成する

	double rate; 
	long sum=0; 
	int i, j ;
	int row , column;

	zval *matrix;
	zval **vector;
	zval **data;
	HashTable *hash_vector;
	HashTable *hash_matrix;


	//-------- 配列要素の受け取り
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a/d/", &matrix,&rate) == FAILURE) 
		return;


	//-------- Eigen行列に値を格納する 
    Eigen::MatrixXf X = read_matrix(matrix,0); 
	row    = X.rows();
	column = X.cols();


	//-------- 特異値分解を実施.　 
	// Eigen3 ありがとう.
    JacobiSVD<MatrixXf> svd(X, ComputeThinU | ComputeThinV);

    MatrixXf U = svd.matrixU();
    MatrixXf S = svd.singularValues();
    MatrixXf V = svd.matrixV();


	//-------- 累積値が指定を満たすように計算し、潜在意味空間を作成する.
	int lsa_value=0;
	double tmp_rate;

	for (i = 0; i < S.size(); i++) {
		lsa_value += 1;
		// 累積値の割合を計算
		MatrixXf SB = S.block(0,0,i+1,1);	
		tmp_rate = SB.sum() / S.sum();

		// 指定した割合に達していなければ、lsa_valueを1増加
    	//std::cout << "rate, lsa_value : " << tmp_rate << " , "<< lsa_value <<std::endl;
		if (rate > tmp_rate) {
			continue;
		// 指定した割合に達していれば、終了.
		}else {
			break;
		}	
	}
	// 部分行列を取り出す.
    MatrixXf UB  = U.block(0,0,row,lsa_value); 
    MatrixXf SD  = S.asDiagonal();
    MatrixXf SDB = SD.block(0,0,lsa_value,lsa_value); 
    MatrixXf SB  = S.block(0,0,lsa_value,1); 
    MatrixXf VB  = V.block(0,0,column,lsa_value); 

    //std::cout << "rate, lsa_value : " << UB <<std::endl;
    //std::cout << "rate, rows : " << UB.rows() <<std::endl;
    //std::cout << "rate, column : " << UB.cols() <<std::endl;

	// 潜在意味空間 LSMの作成
    MatrixXf LSM = UB * SDB *VB.transpose();  


	//-------- 返却するzvalデータを作成する.
	// 1つの配列を作成する.
	//メモリの解放はどうやって、どこで行えばよいか？
	zval *u_matrix;
	zval *s_vector;
	zval *v_matrix;
	zval *lsm_matrix;

	// U_Matrixの作成 
	u_matrix = make_return_matrix(UB);
	s_vector = make_return_vector(SB);
	v_matrix = make_return_matrix(VB);
	lsm_matrix = make_return_matrix(LSM);


	//-------- 返却するオブジェクトを作成する
	zval *result;
	MAKE_STD_ZVAL(result);
	if(object_init(result) != SUCCESS){}
	add_property_zval(result,"U",u_matrix);
	add_property_zval(result,"S",s_vector);
	add_property_zval(result,"V",v_matrix);
	add_property_zval(result,"LSM",lsm_matrix);

	// オブジェクトを返却
	array_init(return_value);
	*return_value = *result;
	zval_copy_ctor(return_value);

	return;
}
PHP_FUNCTION(Eigen_Feature_Vector)
{
	// この関数でやってること
	//-------- １つのベクトル（配列）と対象とする潜在意味空間を取得する.
	//-------- Eigen行列に値を格納する. 
	//-------- 特徴ベクトルを計算する. 
	//-------- 返却するzvalデータを作成する. 
	//-------- 結果を返却する.(配列を返却)

	int row , column;

	zval *matrix;
	zval *s_vector;
	zval *t_vector;

	//-------- １つのベクトル（配列）と対象とする潜在意味空間を取得する.


	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a/a/a/", &matrix,&s_vector,&t_vector) == FAILURE) 
		return;

	//-------- Eigen行列に値を格納する. 
	MatrixXf   Matrix = read_matrix(matrix,1);
	VectorXf S_Vector = read_vector(s_vector,1);
	VectorXf T_Vector = read_vector(t_vector,0);
	MatrixXf S_Matrix = S_Vector.asDiagonal(); 


	//-------- 特徴ベクトルを計算する. 
    //std::cout << "T : " <<std::endl;
    //std::cout <<  T_Vector.transpose() <<std::endl;
    //std::cout << "U : " <<std::endl;
    //std::cout <<  U_Matrix <<std::endl;
    //std::cout << "S : " <<std::endl;
    //std::cout <<  S_Matrix <<std::endl;

	//VectorXf R_Vector = T_Vector.transpose() * (U_Matrix * S_Matrix.inverse());
	MatrixXf R_Matrix = T_Vector.transpose() * (Matrix * S_Matrix.inverse());

    //std::cout << "R : " <<std::endl;
    //std::cout <<  R_Matrix <<std::endl;

	//R_Matrix = Matrix * S_Matrix * R_Matrix.transpose(); 
    //std::cout << "S : " <<std::endl;
    //std::cout <<  S_Matrix <<std::endl;
    //std::cout << "R : " <<std::endl;
    //std::cout <<  R_Matrix <<std::endl;

	//-------- 結果を返却する.(配列を返却)
	zval *result = make_return_vector(R_Matrix);
	*return_value = *result;
	zval_copy_ctor(return_value);

	return;
}
PHP_FUNCTION(Eigen_Inner_Product)
{
	// この関数でやってること
	//-------- 2つのベクトル（配列）を取得する.
	//-------- 配列の要素数を確認し、Eigenベクトルを定義する 
	//-------- Eigenベクトルに値を格納する 
	//-------- 内積をとる,各ベクトルの大きさを計算,cosを計算 
	//-------- 結果を返却する.(doubleを返却)

	zval *vector1;	
	zval *vector2;	

	//-------- 2つのベクトル（配列）を取得する.
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a/a/", &vector1,&vector2) == FAILURE) 
		return;

	//-------- Eigenベクトルを定義する 
	Eigen::VectorXf V1 = read_vector(vector1,1);
	Eigen::VectorXf V2 = read_vector(vector2,1);


	//-------- 内積をとる,各ベクトルの大きさを計算,cosを計算 
	double DOT;
	DOT = V1.dot(V2);

	//-------- 結果を返却する.(doubleを返却)
	RETURN_DOUBLE(DOT);	

}
PHP_FUNCTION(Eigen_COS)
{
	// この関数でやってること
	//-------- 2つのベクトル（配列）を取得する.
	//-------- 配列の要素数を確認し、Eigenベクトルを定義する 
	//-------- Eigenベクトルに値を格納する 
	//-------- 内積をとる,各ベクトルの大きさを計算,cosを計算 
	//-------- 結果を返却する.(doubleを返却)

	zval *vector1;	
	zval *vector2;	

	//HashTable *hash_vector1;
	//HashTable *hash_vector2;

	int array_num;

	//-------- 2つのベクトル（配列）を取得する.
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a/a/", &vector1,&vector2) == FAILURE) 
		return;


	//-------- Eigenベクトルを定義する 
	Eigen::VectorXf V1 = read_vector(vector1,1);
	Eigen::VectorXf V2 = read_vector(vector2,1);


	//-------- 内積をとる,各ベクトルの大きさを計算,cosを計算 
	double DOT;
	DOT = V1.dot(V2);

	double VL1;
	double VL2;
	VL1 = sqrt(V1.array().square().sum());
	VL2 = sqrt(V2.array().square().sum());

	double result;
	result = DOT / (VL1 * VL2);


	//-------- 結果を返却する.(doubleを返却)
	RETURN_DOUBLE(result);	

}
PHP_FUNCTION(Eigen_Vector_Length)
{
	// この関数でやってること
	//-------- 1つのベクトル（配列）を取得する.
	//-------- 配列の要素数を確認し、Eigenベクトルを定義する 
	//-------- Eigenベクトルに値を格納する 
	//-------- ベクトルの大きさを計算
	//-------- 結果を返却する.(doubleを返却)

	zval *vector1;	
	int array_num;

	//-------- 初期化
//	array_init(vector1);

	//-------- 1つのベクトル（配列）を取得する.
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a/", &vector1) == FAILURE) 
		return;


	//-------- 配列の要素数を確認し、Eigenベクトルを定義する 
	Eigen::VectorXf V1 = read_vector(vector1,1);

    //std::cout << "V : " <<std::endl;
    //std::cout <<  V1 <<std::endl;

	//-------- ベクトルの大きさを計算 
	double VL1;
	VL1 = sqrt(V1.array().square().sum());

	//-------- 結果を返却する.(doubleを返却)
	RETURN_DOUBLE(VL1);	

}

//------------ 関数の定義
VectorXf read_vector(zval *vector,int type) {

	HashTable *hash_vector;
	int array_num;


	Eigen::VectorXf V2(1);

	//-------- 配列の要素数を確認し、Eigenベクトルを定義する 
	hash_vector = Z_ARRVAL_P(vector);
	array_num = zend_hash_num_elements(hash_vector);

	// Eigenベクトルを定義 
	Eigen::VectorXf V1(array_num);

	//-------- Eigenベクトルに値を格納する 

	zval **data;
	int i;

	for (i = 0; i < array_num; i++) {
		// 行を読み込み　 
		if (zend_hash_get_current_data(hash_vector, (void **)&data)==FAILURE){
			return V2;
		}
		// Eigenベクトルに値を格納 
		if (type==0) {
			V1(i) = Z_LVAL_PP(data);
		}else if (type==1) {
			V1(i) = Z_DVAL_PP(data);
		}

		// 次の要素に行に移動
		zend_hash_move_forward(hash_vector);
	}
	return V1;

}
MatrixXf read_matrix(zval *matrix,int type) {

	double rate; 
	long sum=0; 
	int i, j ;
	int row , column;

	zval **vector;
	zval **data;
	HashTable *hash_vector;
	HashTable *hash_matrix;

    Eigen::MatrixXf Y(1,1);

	//-------- 初期化
	//zend_hash_init(hash_vector);	
	//zend_hash_init(hash_vector,50,NULL,NULL,false);	
	//zend_hash_init(hash_matrix,50,NULL,NULL,false);	
	//zend_hash_internal_pointer_reset(hash_vector);	
	//zend_hash_internal_pointer_reset(hash_matrix);	

	//-------- 行数・列数を確認し、Eigen行列を定義する 
	// 不完全な行列が渡された場合の対処ができていない.
	hash_matrix = Z_ARRVAL_P(matrix);
	row = zend_hash_num_elements(hash_matrix);
	if (zend_hash_get_current_data(hash_matrix, (void **)&vector)==FAILURE)
		return Y;
	// hashtableに格納　 
	hash_vector = Z_ARRVAL_PP(vector);
	column = zend_hash_num_elements(hash_vector);	

	// 行列を定義 
    Eigen::MatrixXf X(row,column);


	//-------- Eigen行列に値を格納する 
	// hashtableに格納　 
	hash_matrix = Z_ARRVAL_P(matrix);

	for (i = 0; i < row; i++) {
		// 行を読み込み　 
		if (zend_hash_get_current_data(hash_matrix, (void **)&vector)==FAILURE)
			return Y;

		// hashtableに格納　 
		hash_vector = Z_ARRVAL_PP(vector);

		for (j = 0; j < column; j++) {
			// 列を読み込み　 
			if (zend_hash_get_current_data(hash_vector, (void **)&data)==FAILURE)
				return Y;

			// Eigen行列に値を格納する 
			if(type==0){
				X(i,j) = Z_LVAL_PP(data);
			}else if (type==1) {
				X(i,j) = Z_DVAL_PP(data);
			}

			// 次の列に移動
			zend_hash_move_forward(hash_vector);
		}
		// 次の要素に行に移動
		zend_hash_move_forward(hash_matrix);
	}
	return X;	
}
zval* make_return_vector(VectorXf V){
	int i;
	zval *vector;
	ALLOC_INIT_ZVAL(vector);
	array_init(vector);
	for (i = 0; i < V.size(); i++) {
		add_index_double(vector,i,V(i));
	}
	return vector;
}
zval* make_return_matrix(MatrixXf X){
	int i, j;
	zval *matrix;
	zval *tmp_array;

	ALLOC_INIT_ZVAL(matrix);
	array_init(matrix);
	for (i = 0; i < X.rows(); i++) {
		ALLOC_INIT_ZVAL(tmp_array);
		array_init(tmp_array);
		for (j = 0; j < X.cols(); j++) {
			add_index_double(tmp_array,j,X(i,j));
		}
		add_index_zval(matrix,i,tmp_array);
	}
	return matrix;
}
