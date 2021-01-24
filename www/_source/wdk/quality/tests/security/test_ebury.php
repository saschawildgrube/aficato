<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	require_once(GetWDKDir()."wdk_shell.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ebury infection");
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
			
			$this->Trace("This test aims to detect a potential infection by the ebury rootkit on the server.");
			$this->Trace("For more information go to https://www.cert-bund.de/ebury-faq");
			
			$this->Trace("");
			$this->Trace("This test requires that the www user (e.g. www-data) can execute the ipcs command with root privileges.");
			$this->Trace("In order to do this, you'll need to create a file in /etc/sudoers.d/ with mode 0440 that contains the following line:");
			$this->Trace("www-data ALL=(ALL) NOPASSWD: /usr/bin/ipcs");
			$this->Trace("");
			
			$arrayResult = array();
			$bResult = ShellExecute(
				"sudo",
				array("-u root", "ipcs -m"),
				array(),
				"",
				$arrayResult);

			$this->Trace("bResult = ".RenderValue($bResult));				
			$this->Trace($arrayResult);

				
			if ($bResult == false)
			{
				$this->Trace("ShellExecute did not work.");
				$this->SetResult(false);
				return;
			}
			
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			if ($strStdout == "")
			{
				$this->Trace("No shell output. This is unexpected.");
				$this->Trace($arrayResult);
				$this->SetResult(false);
				return;
			}
			
			$arrayResultLines = StringExplode($strStdout,"\n");
			foreach ($arrayResultLines as $strLine)
			{
				if (StringSection($strLine,0,2) == "0x")
				{
					//$this->Trace($strLine);
					//$this->Trace(StringSlice($strLine,StringLength($strLine)));
					$bFound666 = (FindString($strLine,"666") != -1)?(true):(false);

					$strUser = StringTrim(StringSection($strLine,22,10));
					//$this->Trace("User: ".$strUser);
					
					$strPrivileges = StringTrim(StringSection($strLine,33,4));
					//$this->Trace("Privileges: ".$strPrivileges);
					
					$strSize = StringTrim(StringSection($strLine,44,10));
					$nSize = intval($strSize);
					//$this->Trace("Size: ".$nSize);
					
					if (		$strUser == "root"
							&&	$strPrivileges == "666"
							&&	$nSize > 3000000)
					{
						$this->Trace("A shared memory section for root with privileges 666 larger than 3 MB is an indicator for an ebury infection!");	
						$this->SetResult(false);
						return;
					}

				}	
			}
			$this->Trace("No suspicious memory sections were found.");
		}
	}
		
