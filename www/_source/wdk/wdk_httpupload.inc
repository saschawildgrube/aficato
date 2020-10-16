<?php

	function GetUploadedFilesList()
	{
		return array_keys($_FILES);
	}
	
	function GetUploadedFileInfo($strFileID)
	{
		$fileinfo = ArrayGetValue($_FILES,$strFileID);
		if (!is_array($fileinfo))
		{
			return false;
		}
		return $fileinfo;
	}

   function GetUploadedFileName($strFileID)
   {
   	$fileinfo = GetUploadedFileInfo($strFileID);
   	return ArrayGetValue($fileinfo,"name");
   }

   function GetUploadedFilePath($strFileID)
   {
   	$fileinfo = GetUploadedFileInfo($strFileID);
   	return ArrayGetValue($fileinfo,"tmp_name");
   }
   
   function GetUploadedFileSize($strFileID)
   {
   	$fileinfo = GetUploadedFileInfo($strFileID);
   	return ArrayGetValue($fileinfo,"size");
   }   

   function GetUploadedFileMimeType($strFileID)
   {
   	$fileinfo = GetUploadedFileInfo($strFileID);
   	return ArrayGetValue($fileinfo,"type");
   }   


   function GetUploadedFileContents($strFileID)
   {
   	$fileinfo = GetUploadedFileInfo($strFileID);
   	return FileRead(ArrayGetValue($fileinfo,"tmp_name"));
   }
	

