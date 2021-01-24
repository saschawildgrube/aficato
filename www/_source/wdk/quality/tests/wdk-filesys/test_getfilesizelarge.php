<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		var $m_strTmpFile;
		function __construct()
		{
			parent::__construct("GetFileSize (with large files)");
		}
		
		function OnInit()
		{
			$nMemoryLimit = GetMemoryLimit();
			$this->Trace("GetMemoryLimit() = $nMemoryLimit");
			if ($nMemoryLimit < 128000000)
			{
				$this->Trace("Not enough memory to perform this test");
				$this->SetActive(false);		
			}
			
			
			
			return parent::OnInit();	
		}
		

		function TestCase_GetFileSize(
			$strFilePath,
			$nExpectedResult)
		{ 
			$this->Trace("TestCase_GetFileSize");
	
			$this->Trace("strFilePath = \"$strFilePath\"");
			$this->Trace("nExpectedResult: ".RenderValue($nExpectedResult));
			$nResult = GetFileSize($strFilePath);
	
			$this->Trace("GetFileSize() returns: ".RenderValue($nResult));
			
			if (	(($nResult === false) && ($nExpectedResult === false))
				||  ($nResult == $nExpectedResult) )
			{
				$this->Trace("Testcase PASSED!");					
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}
			$this->Trace("");
			$this->Trace("");
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);


			$this->m_strTmpFile = MakeUniqueTempFilePath(GetTempDir());
			$nExpectedSize = 0;  
			$nBlock = 125000000;      
			$data = StringRepeat("a",$nBlock);
			$stopwatch = new CStopWatch();    
			$stopwatch->Start();
			while (
								($stopwatch->GetSeconds() < 30)  
						&&	(GetDiskSpaceFree(GetTempDir()) > 1000000000)
						)
			{
				FileAddText($this->m_strTmpFile,$data);
				$nExpectedSize += $nBlock;
				$this->Trace("Elapsed seconds so far: ".$stopwatch->GetSeconds());
				$this->TestCase_GetFileSize(
					$this->m_strTmpFile,
					$nExpectedSize );
				if ($this->GetResult() == false)
				{
					return;
				}
			}
		}
		
		function OnCleanup()
		{
			DeleteFile($this->m_strTmpFile);			
			return parent::OnCleanup();
		}
		
		
	}
	
	

		
