<?php
	
	require_once("unittest_makesql.inc");
	
	class CTest extends CUnitTestMakeSQL
	{
		function __construct()
		{
			parent::__construct("MakeSQL DELETE");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
				
		function OnTest()
		{
			parent::OnTest();



			$arraySQL = array(
				"command" => "DELETE",
				"tables" => "Table1");
			$this->TestCase_MakeSQL(
				$arraySQL,
				"DELETE FROM /*TablePrefix*/`Table1`;");
				
		$arraySQL = array(
				"command" => "DELETE",
				"tables" => "Table1",
				"where" => "`a` > '1'");
			$this->TestCase_MakeSQL(
				$arraySQL,
				"DELETE FROM /*TablePrefix*/`Table1` WHERE `a` > '1';");				


			$arraySQL = array(
				"command" => "DELETE",
				"tables" => "Table1",
				"where" => array(
					"left" => array(
						"left" => array("field" => "a"),
						"operator" => ">",
						"right" => "1" 
						),
					"operator" => "AND",
					"right" => array(
						"left" => array("field" => "b"),
						"operator" => "=",
						"right" => "bogus" 
						)
					),
				"limit" => array(
					"offset" => 10,
					"rowcount" => 5)
				);
		
			
			$this->TestCase_MakeSQL(
				$arraySQL,
				"DELETE FROM /*TablePrefix*/`Table1` WHERE ((`a` > '1') AND (`b` = 'bogus')) LIMIT 10, 5;");   


			$arraySQL = array(
				"command" => "DELETE",
				"tables" => "Table1",
				"where" => array(
					"left" => array(
						"left" => array("field" => "a"),
						"operator" => ">",
						"right" => "1" 
						),
					"operator" => "AND",
					"right" => array(
						"left" => array("field" => "b"),
						"operator" => "=",
						"right" => "bogus" 
						)
					)
				);
		
			
			$this->TestCase_MakeSQL(
				$arraySQL,
				"DELETE FROM /*TablePrefix*/`Table1` WHERE ((`a` > '1') AND (`b` = 'bogus'));");   
		  
		
		
		/*
		
		
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
		
*/			
		  
		
			$arraySQL = array(
				"command" => "DELETE",
				"tables" => "t1",
				"where" => array(
					"sql" => "`a` = 42"
					)
				);
					
			$this->TestCase_MakeSQL(
				$arraySQL,    
				"DELETE FROM /*TablePrefix*/`t1` WHERE `a` = 42;"); 
		
		}
		

	}
	
	

		
