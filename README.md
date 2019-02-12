# Neural-Network
My assignment for a computer programming course.

---
Application is caplable of:
- Loading network schema from file.
- Training network on training dataset.
- Evaluating the result on the test dataset.

#### Usage example
Printing help
```
CNN.exe --help
```
Training 4x3 cnn with `iris_training.csv` file and evaluating with `iris_eval.csv` file
```
CNN.exe iris_4x3.cnn --train-file=iris_training.csv --eval-file=iris_eval.csv --epochs=25000 --learning-rate=0.0015 --train
```
