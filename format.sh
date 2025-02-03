echo -e "--- start encode conv ---"
python ./Scripts/Format/EncodeConv.py Source/Example
python ./Scripts/Format/EncodeConv.py Source/MechanicEngine
echo -e "--- finish encode conv ---"

echo -e "\n"

echo -e "--- start clang-format ---"
python ./Scripts/Format/FormatFile.py Source/Example
python ./Scripts/Format/FormatFile.py Source/MechanicEngine
echo -e "--- finish clang-format ---"
