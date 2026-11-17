param(
    [Parameter(Mandatory=$true)][string]$Docx,
    [Parameter(Mandatory=$false)][string]$Pdf = ""
)

$ErrorActionPreference = "Stop"
$word = $null
$doc = $null
try {
    $word = New-Object -ComObject Word.Application
    $word.Visible = $false
    $word.DisplayAlerts = 0
    $doc = $word.Documents.Open($Docx, $false, $false)
    $doc.Fields.Update() | Out-Null
    foreach ($toc in $doc.TablesOfContents) {
        $toc.Update() | Out-Null
    }
    $doc.Repaginate()
    $doc.Save()
    if ($Pdf -ne "") {
        $doc.ExportAsFixedFormat($Pdf, 17, $false, 0, 0, 1, 1, $false, $true, $true, 1, $true, $true, $false) | Out-Null
    }
    $pages = $doc.ComputeStatistics(2)
    Write-Output "PAGES=$pages"
    Write-Output "PDF=$Pdf"
}
finally {
    if ($doc -ne $null) { $doc.Close($false) | Out-Null }
    if ($word -ne $null) { $word.Quit() | Out-Null }
}
