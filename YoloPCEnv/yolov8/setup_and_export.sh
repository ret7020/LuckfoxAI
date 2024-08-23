if [ ! -d env ]; then
	python3 -m venv env
fi
	source ./env/bin/activate

if [ ! -d ultralytics_yolov8 ]; then
	git clone https://github.com/airockchip/ultralytics_yolov8
fi

cd ultralytics_yolov8
pip3 install -r requirements.txt
export PYTHONPATH=./
python3 -c "from ultralytics import YOLO; m = YOLO('yolov8n.pt'); m.export(format='onnx')"
echo "Exported model path: ./ultralytics_yolov8/yolov8n.onnx"
