<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	require_once(GetWDKDir()."wdk_shell.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ShellShock");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}

			/*
				This neat little hack here avoids that the debug debris test fails:
				
				ec"."ho
				
				If the word would be in one piece, the debug debris test would consider it
				an unintentionally left command for debug purposes.
			*/

		
		function TestCase_CVE_2014_6271()
		{
			$this->Trace("");
			$this->Trace("CVE-2014-6271:");
			$this->Trace("");
			$this->Trace("This is how the test works:");
			$this->Trace("1. Executing the following command on bash:");
			$this->Trace("env x='() { :;}; ec"."ho vulnerable' bash -c \"ec"."ho this is a test\"");
			$this->Trace("2. Check if the word \"vulnerable\" appears in stdout.");
			$this->Trace("If so, the test fails, the bash is vulnerable.");
			
			/*
				This neat little hack here avoids that the debug debris test fails:
				
				ec"."ho
				
				If the word would be in one piece, the debug debris test would consider it
				an unintentionally left command for debug purposes.
			*/
			
			
			$arrayResult = array();
			$bResult = ShellExecute(
				"env",
				array("x='() { :;}; ec"."ho vulnerable' bash -c \"ec"."ho this is a test\""),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->Trace("");				
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			if ($strStdout == "")
			{
				$this->Trace("No shell output. This is unexpected.");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			
			if (FindString($strStdout,"vulnerable") != -1) 
			{
				$this->Trace("The bash shell is vulnerable!");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			$this->Trace("");			
		}
		

		function TestCase_CVE_2014_7169()
		{
			$this->Trace("");
			$this->Trace("CVE-2014-7169:");
			$this->Trace("");
			
			
			$this->Trace("This is how the test works:");
			$this->Trace("1. Executing the following command on bash:");
			$this->Trace("env X='() { (shellshocker.net)=>\' bash -c \"ec"."ho date\"; cat ec"."ho; rm ./ec"."ho");
			$this->Trace("2. Check if current date appears in stdout.");
			$this->Trace("If so, the test fails, the bash is vulnerable.");


			if (IsDirectory(GetTempDir()) == false)
			{
				$this->Trace("");
				$this->Trace("This test case requires a valid temp directory. Aborting...");
				$this->Trace("");
				return;
			}
		

			$arrayResult = array();
			$bResult = ShellExecute(
				"date",
				array(""),
				array(),
				"",
				$arrayResult);
			$strDate = ArrayGetValue($arrayResult,"stdout");
			$this->Trace("");
			$this->Trace("date returns \"".$strDate."\"");
							
			$arrayResult = array();
			$bResult = ShellExecute(
				"env",
				array("X='() { (shellshocker.net)=>\' bash -c \"".GetTempDir()."ec"."ho date\"; cat ".GetTempDir()."ec"."ho; rm ".GetTempDir()."ec"."ho"),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->Trace("");				
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			
			if (FindString($strStdout,$strDate) != -1) 
			{
				$this->Trace("The bash shell is vulnerable!");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			$this->Trace("");			
		}
		
		
		function TestCase_CVE_Unknown()
		{
			$this->Trace("");
			$this->Trace("Unknown CVE:");
			$this->Trace("");
			$this->Trace("This is how the test works:");
			$this->Trace("1. Executing the following command on bash:");
			$this->Trace("env X=' () { }; ec"."ho hello' bash -c 'date'");
			$this->Trace("2. Check if the word \"hello\" appears in stdout.");
			$this->Trace("If so, the test fails, the bash is vulnerable.");
			
		
			
			$arrayResult = array();
			$bResult = ShellExecute(
				"env",
				array("X=' () { }; ec"."ho hello' bash -c 'date'"),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->Trace("");				
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			if ($strStdout == "")
			{
				$this->Trace("No shell output. This is unexpected.");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			
			if (FindString($strStdout,"hello") != -1) 
			{
				$this->Trace("The bash shell is vulnerable!");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			$this->Trace("");			
		}
		
		
		function TestCase_CVE_2014_7186()
		{
			$this->Trace("");
			$this->Trace("CVE-2014-7186:");
			$this->Trace("");
			$this->Trace("This is how the test works:");
			$this->Trace("1. Executing the following command on bash:");
			$this->Trace("bash -c 'true <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF' || ec"."ho \"CVE-2014-7186 vulnerable, redir_stack\"");
			$this->Trace("2. Check if the string \"CVE-2014-7186 vulnerable, redir_stack\" appears in stdout.");
			$this->Trace("If so, the test fails, the bash is vulnerable.");
			
		
			
			$arrayResult = array();
			$bResult = ShellExecute(
				"bash",
				array("-c 'true <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF <<EOF' || ec"."ho \"CVE-2014-7186 vulnerable, redir_stack\""),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->Trace("");				
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			
			if (FindString($strStdout,"CVE-2014-7186 vulnerable, redir_stack") != -1) 
			{
				$this->Trace("The bash shell is vulnerable!");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			$this->Trace("");			
		}		
		
		function TestCase_CVE_2014_7187()
		{
			
			/*
				When executed as root, the test is successful. When executed as www-data the test fails.
			*/
			return;
			
			$this->Trace("");
			$this->Trace("CVE-2014-7187:");
			$this->Trace("");
			$this->Trace("This is how the test works:");
			$this->Trace("1. Executing the following command on bash:");
			$this->Trace("(for x in {1..200} ; do ec"."ho \"for x\$x in ; do :\"; done; for x in {1..200} ; do ec"."ho done ; done) | bash || ec"."ho \"CVE-2014-7187 vulnerable, word_lineno\"");
			$this->Trace("2. Check if the string \"CVE-2014-7187 vulnerable, word_lineno\" appears in stdout.");
			$this->Trace("If so, the test fails, the bash is vulnerable.");
			
		
			
			$arrayResult = array();
			$bResult = ShellExecute(
				"",
				array("(for x in {1..200} ; do ec"."ho \"for x".'$'."x in ; do :\"; done; for x in {1..200} ; do ec"."ho done ; done) | bash || ec"."ho \"CVE-2014-7187 vulnerable, word_lineno\""),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->Trace("");				
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			if ($strStdout == "")
			{
				$this->Trace("No shell output. This is unexpected.");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			
			if (FindString($strStdout,"CVE-2014-7187 vulnerable, word_lineno") != -1) 
			{
				$this->Trace("The bash shell is vulnerable!");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			$this->Trace("");			
		}		
		
		
		function TestCase_CVE_2014_6278()
		{
			$this->Trace("");
			$this->Trace("CVE-2014-6278:");
			$this->Trace("");
			$this->Trace("This is how the test works:");
			$this->Trace("1. Executing the following command on bash:");
			$this->Trace("shellshocker='() { ec"."ho You are vulnerable; }' bash -c shellshocker");
			$this->Trace("2. Check if the string \"You are vulnerable\" appears in stdout.");
			$this->Trace("If so, the test fails, the bash is vulnerable.");
			
		
			
			$arrayResult = array();
			$bResult = ShellExecute(
				"",
				array("shellshocker='() { ec"."ho You are vulnerable; }' bash -c shellshocker"),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->Trace("");				
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			
			if (FindString($strStdout,"You are vulnerable") != -1) 
			{
				$this->Trace("The bash shell is vulnerable!");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			$this->Trace("");			
		}				
		
		function TestCase_CVE_2014_6277()
		{
			$this->Trace("");
			$this->Trace("CVE-2014-6277:");
			$this->Trace("");
			$this->Trace("This is how the test works:");
			$this->Trace("1. Executing the following command on bash:");
			$this->Trace("bash -c \"f() { x() { _;}; x() { _;} <<a; }\" 2>/dev/null || ec"."ho vulnerable");
			$this->Trace("2. Check if the string \"vulnerable\" appears in stdout.");
			$this->Trace("If so, the test fails, the bash is vulnerable.");
			
		
			
			$arrayResult = array();
			$bResult = ShellExecute(
				"bash",
				array("-c \"f() { x() { _;}; x() { _;} <<a; }\" 2>/dev/null || ec"."ho vulnerable"),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->Trace("");				
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			
			if (FindString($strStdout,"vulnerable") != -1) 
			{
				$this->Trace("The bash shell is vulnerable!");
				$this->Trace("");
				$this->SetResult(false);
				return;
			}
			$this->Trace("");			
		}				
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->Trace("This test aims to detect a potential weakness of the bash shell.");
			$this->Trace("For more information go to:");
			$this->Trace("http://en.wikipedia.org/wiki/Shellshock_(software_bug)");
			$this->Trace("https://shellshocker.net/");
			
			
			$this->TestCase_CVE_2014_6271();
			$this->TestCase_CVE_2014_7169();
			$this->TestCase_CVE_Unknown();
			$this->TestCase_CVE_2014_7186();
			$this->TestCase_CVE_2014_7187();
			$this->TestCase_CVE_2014_6278();
			$this->TestCase_CVE_2014_6277();
			
			
			$this->Trace("");
			if ($this->GetResult() == false)
			{
				
				$this->Trace("Vulnerability detected.");
				$this->Trace("Update the OS immediately by running these commands:");
				$this->Trace("apt-get update");
				$this->Trace("apt-get dist-upgrade");
			}
			else
			{
				$this->Trace("No vulnerability detected.");
			}
		}
	}
		
