<?php
	  //--------------------- Eigen_Singular_Value_Decomposition
	  // 行：文書、列：語句と見立てる
	  $a = array(array(1,1,1),array(2,2,2),array(1,1,1));
	  //$a = array(array(1,0,0),array(0,1,0),array(0,0,1));


	  $obj = Eigen_Singular_Value_Decomposition($a, 0.8); 	
	  echo "---------- U -----------\n";
	  var_dump($obj->U);
	  echo "\n";
	  echo "---------- S -----------\n";
	  var_dump($obj->S);
	  echo "\n";
	  echo "---------- V -----------\n";
	  var_dump($obj->V);
	  echo "\n";
	  echo "--------- LSM ----------\n";
	  var_dump($obj->LSM);
	  echo "\n";


	  //--------------------- Eigen_Feature_Vector
	  // 文書ベクトル
	  $target = array(2,2,2);
	  $res = Eigen_Feature_Vector($obj->V,$obj->S,$target); 	
	  echo "--------- U_vec ----------\n";
	  var_dump($res);
	  echo "\n";


	  // 語句ベクトル
	  //$target = array(1,2,1);
	  //$res = Eigen_Feature_Vector($obj->U,$obj->S,$target); 	
	  //echo "--------- V_vec ----------\n";
	  //var_dump($res);
	  //echo "\n";


	  //--------------------- Eigen_Inner_Product
	  //$b = array(1.1,1.0);	
	  //$c = array(1.1,1.0);	
	  $b = $obj->U[0]; 
	  $c = $res; 
	  $ip  = Eigen_Inner_Product($b,$c);
	  echo "--------- Inner_Product ----------\n";
	  echo $ip."\n";
	  echo "\n";


	  //--------------------- Eigen_Vector_Length
	  $l1  = Eigen_Vector_Length($b);
	  $l2  = Eigen_Vector_Length($c);
	  echo "--------- Vector_Length ----------\n";
	  echo "b length : ".$l1."\n";
	  echo "c length : ".$l2."\n";
	  echo "\n";
	  echo "--------- COS ----------\n";
	  echo "cos :".$ip/($l1*$l2)."\n";
	  echo "\n";

	  //--------------------- Eigen_COS
	  $cos = Eigen_COS($b, $c); 	
	  echo "--------- COS ----------\n";
	  echo "cos :".$cos."\n";
	  echo "\n";




	  //--------------------- Eigen_COS
	  $b = $obj->U[0]; 
	  $c = $res; 
	  $d = $obj->U[1];
	  var_dump($obj->U[0]);
	  var_dump($obj->U[1]);
	  echo "--------- COS ----------\n";
	  echo "cos :".Eigen_COS($b, $c)."\n";
	  echo "cos :".Eigen_COS($d, $c)."\n";
	  echo "\n";


?>
