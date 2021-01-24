<?php

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element TabNavigation");
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

			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-tabnavigation";

			$this->TestCase_CheckURL(
				$strURL,
				array('<table><tbody><tr><td>FIRST_ACTIVE</td><td>ACTIVE:Tab 1</td><td>ACTIVE_TO_INACTIVE</td><td>INACTIVE:<a href="http://www.example.com/2">Tab 2</a></td><td>INACTIVE_TO_INACTIVE</td><td>INACTIVE:<a href="http://www.example.com/3">Tab 3</a></td><td>LAST_INACTIVE</td><td>NO_TAB</td></tr></tbody></table><table><tbody><tr><td>FIRST_INACTIVE</td><td>INACTIVE:<a href="http://www.example.com/1">Tab 1</a></td><td>INACTIVE_TO_ACTIVE</td><td>ACTIVE:Tab 2</td><td>ACTIVE_TO_INACTIVE</td><td>INACTIVE:<a href="http://www.example.com/3">Tab 3</a></td><td>LAST_INACTIVE</td><td>NO_TAB</td></tr></tbody></table><table><tbody><tr><td>FIRST_INACTIVE</td><td>INACTIVE:<a href="http://www.example.com/1">Tab 1</a></td><td>INACTIVE_TO_INACTIVE</td><td>INACTIVE:<a href="http://www.example.com/2">Tab 2</a></td><td>INACTIVE_TO_INACTIVE</td><td>INACTIVE:<a href="http://www.example.com/3">Tab 3</a></td><td>LAST_INACTIVE</td><td>NO_TAB</td></tr></tbody></table>'));
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	
		


		
