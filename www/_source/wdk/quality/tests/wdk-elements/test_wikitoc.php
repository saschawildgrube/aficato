<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Wiki TOC");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			return true;
		}
		
		function TestCase_WikiElement(
			$strURL,
			$strExpected)
		{ 
			$this->Trace("");
			$this->Trace("TestCase_WikiElement");
			$this->TestCase_CheckURL($strURL,array($strExpected));
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);


			$strExpected = '
	<div style="width-max: 50%; margin-top: 5px; margin-bottom: 5px; border: 1px solid #aaaaaa; background-color: #f9f9f9; padding:5px; ">
<a style="text-decoration: none;" href="#header1">1. Header 1</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header11">1.1. Header 1.1</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header111">1.1.1. Header 1.1.1</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header12">1.2. Header 1.2</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header121">1.2.1. Header 1.2.1</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header122">1.2.2. Header 1.2.2</a>
<br/>
<a style="text-decoration: none;" href="#header2">2. Header 2</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header21">2.1. Header 2.1</a>
</div><br/>			
			
			<div id="header1"></div>
<h1>Header 1</h1>

<div id="header11"></div>
<h2>Header 1.1</h2>

<div id="header111"></div>
<h3>Header 1.1.1</h3>

<div id="header12"></div>
<h2>Header 1.2</h2>

<div id="header121"></div>
<h3>Header 1.2.1</h3>

<div id="header122"></div>
<h3>Header 1.2.2</h3>

<div id="header2"></div>
<h1>Header 2</h1>

<div id="header21"></div>
<h2>Header 2.1</h2>
';
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-wiki-toc1";
			$this->TestCase_WikiElement(
				$strURL,
				$strExpected);









			$strExpected = '
	<div style="width-max: 50%; margin-top: 5px; margin-bottom: 5px; border: 1px solid #aaaaaa; background-color: #f9f9f9; padding:5px; ">
<a style="text-decoration: none;" href="#header1">1. Header 1</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header111">1.1. Header 1.1.1</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header12">1.1.1. Header 1.2</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header121">1.1.2. Header 1.2.1</a>
<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a style="text-decoration: none;" href="#header122">1.1.3. Header 1.2.2</a>
</div><br/>


<div id="header1"></div>
<h1>Header 1</h1>

<div id="header111"></div>
<h3>Header 1.1.1</h3>

<div id="header12"></div>
<h2>Header 1.2</h2>

<div id="header121"></div>
<h3>Header 1.2.1</h3>

<div id="header122"></div>
<h3>Header 1.2.2</h3>
';
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-element-wiki-toc2";
			$this->TestCase_WikiElement(
				$strURL,
				$strExpected);


		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		 
		
	} 
	
	
		


		
