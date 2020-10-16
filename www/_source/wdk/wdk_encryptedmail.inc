<?php

	require_once(GetWDKDir()."wdk_encryption-asymmetric.inc");
	
	function SendEncryptedMail(
		&$aee,
		$strPublicKey,
		$arrayRecipientEmailList,
		$strSubject,
		$strMessage,
		$strSenderEmail = "",
		$strSenderName = "",
		$arrayAttachments = array(),
		$arrayRecipientEmailListCC = array(),
		$arrayRecipientEmailListBCC = array())
	{
		$strMessage = StringEncodeUTF8($strMessage);

		// WORKAROUND
		// This avoids that Outlook in combination
		// with GnuPG extention cut's off the mail's text
		// after decryption.
		// It seems as if the decryption with GnuPG confuses byte length
		// and actual length and hence cut's off the amount of characters
		// which are represented as multibyte characters in the encrypted
		// message.
		$nByteLength = strlen($strMessage);
		$nCharLength = mb_strlen($strMessage);
		$strMessage .= str_repeat(" ",$nByteLength-$nCharLength);
		
		$strEncryptedMessage = "";
		$bResult = $aee->Encrypt(
			$strMessage,
			$strPublicKey,
			$strEncryptedMessage);
		if ($bResult == false)
		{
			return false;
		}


		$arrayEncryptedAttachments = array();
		if (is_array($arrayAttachments))
		{
			foreach ($arrayAttachments as $arrayAttachment)
			{
				$strEncryptedFileContent = "";
				$strFileContent = ArrayGetValue($arrayAttachment,"content");
				$bResult = $aee->Encrypt(
					$strFileContent,
					$strPublicKey,
					$strEncryptedFileContent);
				if ($bResult == false)
				{
					//print("Encrypt failed attachment");
					return false;
				}
				$arrayEncryptedAttachments[] = array(
					"name" => ArrayGetValue($arrayAttachment,"name"),
					"content" => $strEncryptedFileContent);
			}			
		}


		$bResult = SendMail(
			$arrayRecipientEmailList,
			$strSubject,
			$strEncryptedMessage,
			$strSenderEmail,
			$strSenderName,
			$arrayEncryptedAttachments,
			$arrayRecipientEmailListCC,
			$arrayRecipientEmailListBCC);
	
		
		return $bResult;
	}	
	
		
