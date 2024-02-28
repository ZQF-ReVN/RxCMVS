# CMVS_Tools
Tools For Purple software's CMVS Engine.  

## File Extension 
`.ps2` `.ps3` `.pb3` `.cpz`

## Tools
### PS3_TextEditor
- export msg text from `.ps3` `.ps2` file
- import msg text to `.ps3` `.ps2` file

### PS3_Cryptor
- Decrypt `.ps3` `.ps2` file
- Encrypt `.ps3` `.ps2` file (fake encryption)

### MGV_Editor
- unpack `.ogv` video file and `.ogg` audio file from `.mgv` file
- replace `.ogv` video file

### VFS_Extract
extract file by file name via hook the function that create pack stream to get the pack stream obj.

### VFS_Hook
patch files without repack via hook the interface for reading files.

### CMV6_Hook
- [x] unpack .CMV
- [x] decode .JBPD (dynamic for now)
- [x] decode .JBPD to BitMap
- [x] decode .JBPD to WebP
- [x] repack .CMV
- [x] play .CMV with WebPDecoder

## Essays
[[CMVS] 文件读取分析](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/Reverse/%5BCMVS%5D%20%E6%96%87%E4%BB%B6%E8%AF%BB%E5%8F%96%E5%88%86%E6%9E%90/CMVS%20%E6%96%87%E4%BB%B6%E8%AF%BB%E5%8F%96%E5%88%86%E6%9E%90.md)  
[[CMVS] 紫社引擎 折腾日记 Purple software](https://github.com/Dir-A/Dir-A_Essays_MD/blob/main/Reverse/%5BCMVS%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software/%5BCMVS%5D%20%E7%B4%AB%E7%A4%BE%E5%BC%95%E6%93%8E%20%E6%8A%98%E8%85%BE%E6%97%A5%E8%AE%B0%20Purple%20software.md)  

## More
Check the details of reverse analysis [note](/etc)  

## Tested
クナド国記 PKG  
青春フラジャイル PKG  
リアライブ PKG  
アオイトリ PKG  
アマツツミ PKG  
ハピメアFD ハピメアＷパック PKG  
クロノクロック PKG  
ハピメア PKG  
しあわせ家族部 PKG  
未来ノスタルジア PKG  
