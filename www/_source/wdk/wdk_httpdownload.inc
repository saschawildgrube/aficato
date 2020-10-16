<?php

	function DownloadFileAndExit(
		$strMimeType,
		$strFileName,
		$strContents)
	{
		header("Content-type: $strMimeType");
		header("Content-Disposition: attachment; filename=\"$strFileName\"");
		header("Content-Length: ".strlen($strContents));
		print($strContents);
		exit();	
	}
	


