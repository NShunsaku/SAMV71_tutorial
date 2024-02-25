$today = Get-Date -Format "yyyyMMdd"
git add .
git commit -m "ver.$today"
git push -uf origin main
