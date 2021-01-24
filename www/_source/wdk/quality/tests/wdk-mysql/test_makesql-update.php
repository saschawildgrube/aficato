<?php
	
	require_once("unittest_makesql.inc");
	
	class CTest extends CUnitTestMakeSQL
	{
		function __construct()
		{
			parent::__construct("MakeSQL UPDATE");
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
				"command" => "UPDATE",
				"tables" => "Table1",
				"values" => array(
					array(
						"field" => "a",
						"value" => 1),
					array(
						"field" => "b",
						"value" => "Hallo Welt"),
					),
				"where" => array(
					"and" => array(
						array(
							"left" => array(
								"field" => "a"
								),
							"operator" => ">",
							"right" => 1 
							),
						array(
							"left" => array(
								"field" => "b"
								),
							"operator" => "=",
							"right" => "Blubb" 
							)
						)
					),
				"orderby" => array(
					array(
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
				"UPDATE /*TablePrefix*/`Table1` SET `a` = 1, `b` = 'Hallo Welt' WHERE (`a` > 1) AND (`b` = 'Blubb') ORDER BY `a` DESC LIMIT 10, 5;"); 
		  
		
		
			$arraySQL = array(
				"command" => "UPDATE",
				"tables" => "Table1",
				"values" => array(
					array(
						"field" => "a",
						"value" => 1),
					array(
						"field" => "b",
						"value" => "Hallo Welt"),
					),
				"where" => array(
					"and" => array(
						array(
							"left" => array(
								"field" => "a"
								),
							"operator" => ">",
							"right" => 1 
							),
						array(
							"left" => array(
								"field" => "b"
								),
							"operator" => "=",
							"right" => "Blubb" 
							)
						)
					),
				"limit" => array(
					"rowcount" => 2)
				);
		
			
			$this->TestCase_MakeSQL(
				$arraySQL,
				"UPDATE /*TablePrefix*/`Table1` SET `a` = 1, `b` = 'Hallo Welt' WHERE (`a` > 1) AND (`b` = 'Blubb') LIMIT 2;"); 
		  		

			$arraySQL = array(
				"command" => "UPDATE",
				"tables" => "Table1",
				"values" => array(
					array(
						"field" => "a",
						"value" => 0)
					),
				"where" => array(
					"and" => array(
						array(
							"left" => array(
								"field" => "a"
								),
							"operator" => ">",
							"right" => 0 
							)
						)
					),
				"limit" => array(
					"rowcount" => 2)
				);
		
			
			$this->TestCase_MakeSQL(
				$arraySQL,
				"UPDATE /*TablePrefix*/`Table1` SET `a` = 0 WHERE (`a` > 0) LIMIT 2;"); 
		
		
		}
		

	}
	
	

		
