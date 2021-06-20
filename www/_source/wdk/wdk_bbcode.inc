<?php

	function StringBBCodeToHtml($strHTML, $strCSSClass = "")
	{
		$strClassAttribute = ($strCSSClass == "")?(""):(" class=\"$strCSSClass\"");
		$strHTML = StringRemoveControlChars($strHTML);
		$strHTML = ReplaceString($strHTML,"[br]","<br".$strClassAttribute."/>");
		$strHTML = ReplaceString($strHTML,"[*]","<li".$strClassAttribute.">");
		while (preg_match_all('`\[(.+?)=?(.*?)\](.+?)\[/\1\]`', $strHTML, $arrayMatches))
		{
			foreach ($arrayMatches[0] as $strKey => $strMatch)
			{ 
				$strTag = $arrayMatches[1][$strKey];
				$strParam = $arrayMatches[2][$strKey];
				$strInline = $arrayMatches[3][$strKey];
            switch ($strTag)
            { 
					case 'b':	
						$strReplacement =
							"<strong".$strClassAttribute.">".$strInline."</strong>";
						break; 
					case 'i':
						$strReplacement =
							"<em".$strClassAttribute.">".$strInline."</em>";
						break; 
					case 'size':
						$strReplacement =
							"<span style=\"font-size: ".$strParam."px;\"".$strClassAttribute.">".$strInline."</span>";
						break; 
					case 'color':
						$strReplacement =
							"<span style=\"color: ".$strParam.";\"".$strClassAttribute.">".$strInline."</span>";
						break; 
					case 'center':
						$strReplacement =
							"<div style=\"margin-left:auto;margin-right:auto;\"".$strClassAttribute.">".$strInline."</div>";
						break; 
					case 'quote':
						$strReplacement =
							"<blockquote".$strClassAttribute.">".$strInline."</blockquote>" .
							(($strParam)?("<cite>".$strParam."</cite>"):(''));
						break; 
					case 'url':
						$strURL = (($strParam != "") ? ($strParam) : ($strInline));
						$arrayURL = ParseURL($strURL);
						$strProtocol = ArrayGetValue($arrayURL,"PROTOCOL");
						if ($strProtocol == "")
						{
							$strURL = "http://".$strURL;	 
						}
						$strReplacement =
							'<a href="'.
							$strURL.
							"\" target=\"_blank\"".$strClassAttribute.">".$strInline."</a>";
						break; 
					case 'list':
						$strReplacement =
							'<ul'.$strClassAttribute.'>'.
							$strInline.
							"</li></ul>";
						break;
					case 'table':
						$strReplacement =
							'<table'.$strClassAttribute.'><tbody>'.
							$strInline.
							"</tbody></table>";
						break;
					case 'tr':
						$strReplacement =
							'<tr'.$strClassAttribute.'>'.
							$strInline.
							"</tr>";
						break;
					case 'td':
						$strReplacement =
							'<td'.$strClassAttribute.'>'.
							$strInline.
							"</td>";
						break;
					case 'th':
						$strReplacement =
							'<th'.$strClassAttribute.'>'.
							$strInline.
							"</th>";
						break;	
					case 'img':  
						list($width, $height) = preg_split('`[Xx]`', $param); 
						$strReplacement =
							"<img src=\"".$strInline."\" ".
							(is_numeric($width)? "width=\"".$width."\" " : '').
							(is_numeric($height)? "height=\"".$height."\" " : '').
							'/>'; 
                	break; 
					case 'video': 
						//$strVideoURL = (($strParam != "") ? ($strParam) : ($strInline));
						$strVideoURL = $strInline;
						$arrayVideourl = ParseURL($strVideoURL);
						if (is_array($arrayVideourl) == false)
						{
							break;	
						} 
						$arrayVideoquery = ParseURLParameters(ArrayGetValue($arrayVideourl,'PARAMETERS')); 
						$strHost = ArrayGetValue($arrayVideourl,'HOST');
						if (FindString($strHost, 'youtube.com') != -1)
						{
							$strReplacement =
								'<embed src="http://www.youtube.com/v/'.
								ArrayGetValue($arrayVideoquery,'v').
								'" type="application/x-shockwave-flash" width="425" height="344"'.$strClassAttribute.'></embed>';
						}
						if (FindString($strHost, 'google.com') != -1)
						{
							$strReplacement =
								'<embed src="http://video.google.com/googleplayer.swf?docid='.
								ArrayGetValue($arrayVideoquery,'docid').
								'" width="400" height="326" type="application/x-shockwave-flash"'.$strClassAttribute.'></embed>';
						}
                break; 
				} 
				$strHTML = ReplaceString($strHTML, $strMatch, $strReplacement);
			} 
		}
		return $strHTML; 
	} 

