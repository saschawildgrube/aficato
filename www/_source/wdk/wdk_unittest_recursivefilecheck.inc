<?php

	require_once("wdk_unittest.inc");
		
	abstract class CUnitTestRecursiveFileCheck extends CUnitTest
	{
		function __construct($strName,$arrayConfig = array())
		{
			parent::__construct(
				$strName,
				$arrayConfig);
		}
		
		
		function CheckFileAgainstRegExp($strFilePath,$arrayRegExp,$bExpectedMatch = false)
		{
			if (!is_array($arrayRegExp))
			{
				return;	
			}
			if (ArrayCount($arrayRegExp) == 0)
			{
				return;	
			}
			$strFileContent = FileRead($strFilePath);
			foreach ($arrayRegExp as $strRegExp)
			{
				if (RegExpMatch($strFileContent,$strRegExp) != $bExpectedMatch)
				{
					$this->Trace("strFilePath = \"$strFilePath\"");
					$this->Trace("strRegExp = \"$strRegExp\"");
					$this->Trace("bExpectedMatch = ".RenderBool($bExpectedMatch));
					if ($bExpectedMatch == false)
					{
						$this->Trace("The regular expression matched in the file.");
					}
					else
					{
						$this->Trace("The regular expression WAS NOT matched in the file.");
					}
					$this->Trace("Testcase FAILED!");	
					$this->Trace("");
					$this->Trace("");
					$this->SetResult(false);
				}
				
			}

		}
		
		function Callback_TestCase_CheckFile($strFilePath)
		{
			
		}

		function Callback_TestCase_CheckFolder($strFolderPath)
		{
			
		}
		
		final
		function CheckFile($strFilePath)
		{
			$this->Callback_TestCase_CheckFile($strFilePath);
		}

		final
		function CheckFolder($strFolderPath)
		{
			$this->Callback_TestCase_CheckFolder($strFolderPath);
		}

		
		function CheckDirectory($strDir,$bRecursive)
		{
			$this->Trace("");
			$this->Trace("CheckDirectory(\"$strDir\")");
			$this->Trace("");
			$this->Trace("");
			if (IsDirectory($strDir) == false)
			{
				$this->Trace("IsDirectory(\"$strDir\") returned false.");
				return;
			}
			$arrayFiles = ReadDirectory($strDir,$bRecursive);
			foreach ($arrayFiles as $strFile)
			{
				$strPath = $strDir . $strFile;
				if (IsFile($strPath))
				{
					$this->CheckFile($strPath);
				}
				if (IsDirectory($strPath))
				{
					$this->CheckFolder($strPath);
				}

			}
			$this->Trace("");
		}
		
		function CheckSourceDirectories()
		{
			$this->CheckDirectory(GetDocumentRootDir(),false);

			$this->CheckDirectory(GetSourceDir(),true);
			if (FindString(GetWDKDir(),GetSourceDir()) == -1)
			{
				$this->Trace("GetWDKDir() is not within GetSourceDir()");
				$this->CheckDirectory(GetWDKDir(),true);
			}

			$this->CheckDirectory(GetWebservicesDir(),true);
			
			$this->CheckDirectory(GetDocumentRootDir()."install/",true);
			$this->CheckDirectory(GetDocumentRootDir()."quality/",true);

		}

		function CheckDocumentRootDirectory()
		{
			$this->CheckDirectory(GetDocumentRootDir(),true); 
		}


	}

