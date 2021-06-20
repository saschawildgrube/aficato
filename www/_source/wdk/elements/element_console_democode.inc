$this->IncludeElement("console");

$strLog = "
2008-12-30 12:00:00 Something happened
2008-12-30 12:00:01 More stuff happened
2008-12-30 12:01:01 Some <xml/> tag is included in this log entry
2008-12-30 12:05:00 This is some blind text that is very long and definitly exceeds the typical width of a console line.
2008-12-30 12:05:01 Evenmoreverylongtextwithoutanyspacestodemonstratehowandiftextisbeingwordwrappedifthelineoftextistoolongtofitintooncelineofcode.Thebigchallengeistofindthebestplacetoaddawordwrap.
2008-12-31 23:59:59 Say goodbye to the old year
";

$strOutput = "";

$console = new CConsoleElement($this->GetWebsite(),"default");
$strOutput .= $console->RenderConsole($strLog);