if [ ! -d env ]; then
	python3 -m venv env
fi
	source ./env/bin/activate

if [ ! -d yolov5 ]; then
	git clone https://github.com/airockchip/yolov10.git
fi

cd yolov5
pip3 install -r requirements.txt
python3 export.py --rknpu --weight yolov5n.pt
echo "Exported model path: ./yolov5/yolov5n.onnx"
