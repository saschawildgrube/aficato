<?php
	
	require_once("unittest_makesql.inc");
	
	class CTest extends CUnitTestMakeSQL
	{
		function __construct()
		{
			parent::__construct("MakeSQL SELECT");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		

		
		function CallbackTest()
		{
			parent::CallbackTest();

			$arraySQL = array(
				"command" => "SELECT",
				"tables" => "Table1");
			$this->TestCase_MakeSQL(
				$arraySQL,
				"SELECT * FROM /*TablePrefix*/`Table1`;");


			$arraySQL = array(
				"command" => "SELECT",
				"fields" => array(
					array(
						"table" => "t1",
						"field" => "a"),
					array(
						"table" => "t2",
						"field" => "b"),
					),
				"tables" => array(
					array(
						"table" => "Table1",
						"as" => "t1"
						),
					array(
						"join" => "LEFT JOIN",
						"table" => "Table2",
						"as" => "t2"
						),
					array(
						"join" => "LEFT JOIN",
						"table" => "Table3",
						"as" => "t3",
						"on" => array(
							"left" => array(
								"table" => "t1",
								"field" => "a"),
							"operator" => "=",
							"right" => array(
								"table" => "t3",
								"field" => "a")								
							)
						)
					),
				"where" => array(
					"left" => array(
						"left" => array(
							"table" => "t1",
							"field" => "a"
							),
						"operator" => ">",
						"right" => "1" 
						),
					"operator" => "AND",
					"right" => array(
						"left" => array(
							"table" => "t1",
							"field" => "c"
							),
						"operator" => "=",
						"right" => "bogus" 
						)
					),
				"groupby" => array(
					array(
						"table" => "t1",
						"field" => "version"
						)
					),
				"orderby" => array(
					array(
						"table" => "t1",
						"field" => "a",
						"order" => "DESC"
						)
					),
				"limit" => array(
					"offset" => 10,
					"rowcount" => 5)
				);
		
			
			$this->TestCase_MakeSQL(
				$arraySQL,
				"SELECT /*TablePrefix*/`t1`.`a`, /*TablePrefix*/`t2`.`b` FROM /*TablePrefix*/`Table1` AS /*TablePrefix*/`t1` LEFT JOIN /*TablePrefix*/`Table2` AS /*TablePrefix*/`t2` LEFT JOIN /*TablePrefix*/`Table3` AS /*TablePrefix*/`t3` ON (/*TablePrefix*/`t1`.`a` = /*TablePrefix*/`t3`.`a`) WHERE ((/*TablePrefix*/`t1`.`a` > '1') AND (/*TablePrefix*/`t1`.`c` = 'bogus')) GROUP BY /*TablePrefix*/`t1`.`version` ORDER BY /*TablePrefix*/`t1`.`a` DESC LIMIT 10, 5;"); 
		  
		
		
		
		
		
			$arraySQL = array(
				"command" => "SELECT",
				"calculate_found_rows" => true,
				"fields" => array(
					array(
						"table" => "t1",
						"field" => "a"),
					array(
						"table" => "t2",
						"field" => "b",
						"as" => "a2"),
					),
				"tables" => array(
					array(
						"table" => "Table1",
						"as" => "t1"
						),
					array(
						"join" => "LEFT JOIN",
						"table" => "Table2",
						"as" => "t2"
						),
					array(
						"join" => "LEFT JOIN",
						"table" => "Table3",
						"as" => "t3",
						"on" => array(
							"left" => array(
								"table" => "t1",
								"field" => "a"),
							"operator" => "=",
							"right" => array(
								"table" => "t3",
								"field" => "a")								
							)
						)
					),
				"where" => array(
					"and" => array(
						array(
							"left" => array(
								"table" => "t1",
								"field" => "a"
								),
							"operator" => ">",
							"right" => "1" 
							),
						array(
							"left" => array(
								"table" => "t1",
								"field" => "b"
								),
							"operator" => "=",
							"right" => "2" 
							),
						array(
							"left" => array(
								"table" => "t1",
								"field" => "c"
								),
							"operator" => "=",
							"right" => "bogus" 
							)
						)
					),
				"groupby" => array(
					array(
						"table" => "t1",
						"field" => "version"
						)
					),
				"orderby" => array(
					array(
						"table" => "t1",
						"field" => "a",
						"order" => "DESC"
						)
					),
				"limit" => array(
					"offset" => 10,
					"rowcount" => 5)
				);
		
			
			$this->TestCase_MakeSQL(
				$arraySQL,
				"SELECT SQL_CALC_FOUND_ROWS /*TablePrefix*/`t1`.`a`, /*TablePrefix*/`t2`.`b` AS `a2` FROM /*TablePrefix*/`Table1` AS /*TablePrefix*/`t1` LEFT JOIN /*TablePrefix*/`Table2` AS /*TablePrefix*/`t2` LEFT JOIN /*TablePrefix*/`Table3` AS /*TablePrefix*/`t3` ON (/*TablePrefix*/`t1`.`a` = /*TablePrefix*/`t3`.`a`) WHERE (/*TablePrefix*/`t1`.`a` > '1') AND (/*TablePrefix*/`t1`.`b` = '2') AND (/*TablePrefix*/`t1`.`c` = 'bogus') GROUP BY /*TablePrefix*/`t1`.`version` ORDER BY /*TablePrefix*/`t1`.`a` DESC LIMIT 10, 5;"); 
		  
		
		
		
		
		
			$arraySQL = array(
				"command" => "SELECT",
				"calculate_found_rows" => true,
				"fields" => array(
					array(
						"table" => "t1",
						"field" => "a"),
					array(
						"table" => "t2",
						"field" => "b",
						"as" => "a2"),
					),
				"tables" => array(
					array(
						"table" => "Table1",
						"as" => "t1"
						),
					array(
						"join" => "LEFT JOIN",
						"table" => "Table2",
						"as" => "t2"
						),
					array(
						"join" => "LEFT JOIN",
						"table" => "Table3",
						"as" => "t3",
						"on" => array(
							"left" => array(
								"table" => "t1",
								"field" => "a"),
							"operator" => "=",
							"right" => array(
								"table" => "t3",
								"field" => "a")								
							)
						)
					),
				"where" => array(
					"and" => array(
						array(
							"left" => array(
								"table" => "t1",
								"field" => "a"
								),
							"operator" => ">",
							"right" => "1" 
							),
						array(
							"left" => array(
								"table" => "t1",
								"field" => "b"
								),
							"operator" => "=",
							"right" => "2" 
							),
						array(
							"left" => array(
								"table" => "t1",
								"field" => "c"
								),
							"operator" => "=",
							"right" => "bogus" 
							)
						)
					),
				"groupby" => array(
					array(
						"table" => "t1",
						"field" => "version"
						)
					),
				"limit" => array(
					"rowcount" => 5)
				);
		
			
			$this->TestCase_MakeSQL(
				$arraySQL,
				"SELECT SQL_CALC_FOUND_ROWS /*TablePrefix*/`t1`.`a`, /*TablePrefix*/`t2`.`b` AS `a2` FROM /*TablePrefix*/`Table1` AS /*TablePrefix*/`t1` LEFT JOIN /*TablePrefix*/`Table2` AS /*TablePrefix*/`t2` LEFT JOIN /*TablePrefix*/`Table3` AS /*TablePrefix*/`t3` ON (/*TablePrefix*/`t1`.`a` = /*TablePrefix*/`t3`.`a`) WHERE (/*TablePrefix*/`t1`.`a` > '1') AND (/*TablePrefix*/`t1`.`b` = '2') AND (/*TablePrefix*/`t1`.`c` = 'bogus') GROUP BY /*TablePrefix*/`t1`.`version` LIMIT 5;"); 
		  
				
		
		
		
		
		
		
			$arraySQL = array(
				"command" => "SELECT",
				"tables" => "t1",
				"where" => array(
					"sql" => "`a` = 42"
					)
				);
					
			$this->TestCase_MakeSQL(
				$arraySQL,
				"SELECT * FROM /*TablePrefix*/`t1` WHERE `a` = 42;"); 



	
		$arraySQL = array(
				"command" => "SELECT",
				"tables" => "t1",
				"where" => array(
					"left" => array(
						//"table" => "t1",
						"field" => "a"),
					"operator" => "IN",
					"right" => array(
						"values" => array("1","2","3"))
					)
				);
					
			$this->TestCase_MakeSQL(
				$arraySQL,
				"SELECT * FROM /*TablePrefix*/`t1` WHERE (`a` IN ('1', '2', '3'));"); 

	
		
		}
		

	}
	
	

		
