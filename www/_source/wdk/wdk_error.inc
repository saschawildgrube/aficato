<?php

	class CErrorAmnesty
	{
		private $m_errorReporting;
		private $m_strDisplayErrors;
		
		function __construct()
		{
			$this->m_bActive = false;
		}
		
		function Start()
		{
			if ($this->m_bActive == true)
			{
				return;	
			}
			$this->m_errorReporting = error_reporting (0);
			$this->m_strDisplayErrors = ini_set('display_errors', 0);
			$this->m_bActive = true;
		}
		
		function End()
  	{
  		if ($this->m_bActive == false)
  		{
  			return;	
  		}
			error_reporting ($this->m_errorReporting);
			ini_set('display_errors', $this->m_strDisplayErrors);
			$this->m_bActive = false;
		}
	}
	
	
	function FatalError($strText,$strDetails = "")
	{
		$strMail = GetAdminEmail();
		if ($strMail != "")
		{
			SendMail(
					$strMail,
					"FATAL ERROR: ".$strText,
					$strText."\n\n".$strDetails,
					GetSystemEmail(),
					GetSystemEmailSenderName());
		}
		print(MakeDocType("xhtml11").
"<html>
	<head>
		<title>".HtmlEncode($strText)."</title>
	</head>
	<body>
		<h1>FATAL ERROR</h1><p>".HtmlEncode($strText)."</p>
		<pre>".HtmlEncode($strDetails)."</pre>
	</body>
</html>");
		exit();		
	}

