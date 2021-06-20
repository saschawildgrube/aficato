<?php

	define("REGEXP_EMAIL", "/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,15})$/");

	function IsValidEmail($strEmail)
	{
		$strEmail = StringEncodeUTF8($strEmail);
		$nEmailLength	= StringLength($strEmail);
		if ($nEmailLength < EMAIL_MINLEN)
		{
			return false;
		}
		if ($nEmailLength > EMAIL_MAXLEN)
		{
			return false;
		}
		if (!StringCheckCharSet($strEmail,CHARSET_EMAIL))
		{
			return false;	
		}
		$strEmail = StringLowerCase($strEmail);
		if (RegExpMatch($strEmail,REGEXP_EMAIL) == false)  
		{
			return false;			
		}
		if (StringSection($strEmail,-4) == ".css")
		{
			return false;	
		}
		if (StringSection($strEmail,-3) == ".js")
		{
			return false;	
		}		
		if (StringSection($strEmail,-3) == ".png")
		{
			return false;	
		}		
		if (StringSection($strEmail,-3) == ".jpg")
		{
			return false;	
		}		
		if (StringSection($strEmail,-3) == ".pdf")
		{
			return false;	
		}		
		return true;
	}

	function IsValidEmailURL($strURL)
	{
		if (StringSection($strURL,0,7) != "mailto:")
		{
			return false;	
		}
		return IsValidEmail(StringSection($strURL,7));
	}
	
	
	
	function SendMail(
		$arrayRecipients,
		$strSubject,
		$strMessage,
		$strSenderEmail = "",
		$strSenderName = "",
		$arrayAttachments = array(),
		$arrayRecipientsCC = array(),
		$arrayRecipientsBCC = array())
	{
		
		if (is_array($arrayRecipients) != true)
		{
			$arrayRecipients = array($arrayRecipients);	
		}
		foreach ($arrayRecipients as $strRecipientEmail)
		{
			if (!IsValidEmail($strRecipientEmail))
			{
				return false;
			}
		}
		$strRecipients = implode(",",$arrayRecipients);
		
		$strHeaderCCBCC = "";

		if (is_array($arrayRecipientsCC) != true)
		{
			$arrayRecipientsCC = array($arrayRecipientsCC);	
		}
		foreach ($arrayRecipientsCC as $strRecipientEmailCC)
		{
			if (!IsValidEmail($strRecipientEmailCC))
			{
				return false;
			}
		}
		if (ArrayCount($arrayRecipientsCC) > 0)
		{
			$strHeaderCCBCC .= "Cc:".implode(",",$arrayRecipientsCC)."\n";
		}

		if (is_array($arrayRecipientsBCC) != true)
		{
			$arrayRecipientsBCC = array($arrayRecipientsBCC);	
		}
		foreach ($arrayRecipientsBCC as $strRecipientEmailBCC)
		{
			if (!IsValidEmail($strRecipientEmailBCC))
			{
				return false;
			}
		}
		if (ArrayCount($arrayRecipientsBCC) > 0)
		{
			$strHeaderCCBCC .= "Bcc:".implode(",",$arrayRecipientsBCC)."\n";
		}

		if ($strSubject != "")
		{
			$strSubject = "=?UTF-8?B?".base64_encode(StringEncodeUTF8($strSubject))."?=";
		}

		$strMessage = ReplaceString($strMessage,"\r","");
		$strMessage = StringEncodeUTF8($strMessage);

		if ($strSenderName != "")
		{
			$strSenderName = "=?UTF-8?B?".base64_encode(StringEncodeUTF8($strSenderName))."?=";
		}
		
		
		if ($strSenderEmail != "")
		{
			if (IsValidEmail($strSenderEmail) != true)  
			{
				return false;
			}	
		}
			
		$bAttachments = false;
		
		if (is_array($arrayAttachments))
		{
			if (ArrayCount($arrayAttachments) > 0)
			{
				foreach ($arrayAttachments as $attachment)
				{
					if (ArrayGetValue($attachment,"name") == "")
					{
						return false;	
					}
					if (strlen(ArrayGetValue($attachment,"content")) == 0)
					{
						return false;	
					}
				}
				$bAttachments = true;
			}
		}

		$strHeaders = '';		
		$strBody = '';		
		if ($bAttachments == false)
		{
			$strHeaders .= "Content-type: text/plain; charset=utf-8\n";
			$strHeaders .= "From: $strSenderName <$strSenderEmail>\n";
			$strHeaders .= "Return-Path: $strSenderEmail\n";
			$strHeaders .= "Reply-To: $strSenderEmail\n";
			$strHeaders .= $strHeaderCCBCC;
			$strHeaders .= "MIME-Version: 1.0\n"; 
  		//$strHeaders .= "Content-Transfer-Encoding: quoted-printable\n";
  		$strHeaders = trim($strHeaders);
  		
  		$strBody = $strMessage;
		}
		else
		{
			$strBoundary = rand(0,9)."-".rand(10000000000,9999999999)."-".rand(10000000000,9999999999)."=:".rand(10000,99999);

			$strHeaders = "From: $strSenderName <$strSenderEmail>\n";
			$strHeaders .= "Return-Path: $strSenderEmail\n";
			$strHeaders .= "Reply-To: $strSenderEmail\n";
			$strHeaders .= $strHeaderCCBCC;
			$strHeaders .= "MIME-Version: 1.0\n";
			$strHeaders .= "Content-Type: multipart/mixed;\n";
			$strHeaders .= "    boundary=\"$strBoundary\"\n";
			$strHeaders = trim($strHeaders);
			
			$strBody =  "\nThis is a multi-part message in MIME format.\n\n";
			$strBody .= "--$strBoundary\n";
			/*
			$strBody .= "Content-Type: text/plain;\n";
	    	$strBody .= "charset=\"windows-1256\"\n";
	    	*/
	    $strBody .= "Content-Type: text/plain; charset=\"utf-8\"\n";
			//$strBody .= "Content-Transfer-Encoding: quoted-printable\n";
			$strBody .= "\n$strMessage\n\n";
			foreach ($arrayAttachments as $attachment)
			{
				$strBody .= "--$strBoundary\n";
				$strBody .= "Content-Type: text/text\n";
				$strBody .= "    name: \"".ArrayGetValue($attachment,"name")."\"\n";
	    	$strBody .= "Content-Transfer-Encoding: base64\n";
				$strBody .= "Content-Disposition: attachment;\n";
			  $strBody .= "    filename=\"".ArrayGetValue($attachment,"name")."\"\n\n";
			  $strBody .= chunk_split(base64_encode(ArrayGetValue($attachment,"content")),76,"\n")."\n";
			}
			$strBody .= "--$strBoundary--\n";
		}	
		
		$erroramnesty = new CErrorAmnesty();
		$erroramnesty->Start();
				
		$nRemainingAttempts = 2;  
		while ($nRemainingAttempts > 0)
		{
			$nRemainingAttempts--;
			
			$bResult = mail(
				$strRecipients,
				$strSubject,
				$strBody,
				$strHeaders);
				
			if ($bResult == true)
			{
				$nRemainingAttempts = 0;
			}
		}
		$erroramnesty->End();
				
		return $bResult;
	}	
	
		
