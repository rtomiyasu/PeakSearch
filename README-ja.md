[to Engilish](https://github.com/rtomiyasu/PeakSearch/blob/main/README.md)
# PeakSearch CUI プログラムの操作説明書
## 概要
以下では、オープンソースのPeak search プログラム[Version 0.99](https://github.com/rtomiyasu/PeakSearch/tree/main/PeakSearch0_9_99_win)について簡単に説明します。粉末指数づけソフトウェアConograph GUIに同プログラムは付属しており、Conograph CUIの入力ファイルを作成するために使用します。同プログラムは、Figure 1のような2次元データのピーク探索を実行します。

![Graph0](https://github.com/rtomiyasu/PeakSearch/assets/149344913/4c71a7c7-1d95-4a40-99c6-12da01b5950f)
```
Figure 1 : ２次元データの例
```

実行後、入力された2次元データのコピーおよび、入力で指定したしきい値以上のピーク高さを持つと推定されるピークに関する以下の情報を含む、IGORテキストファイル([例](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/output/sample1_pks.histogramIgor))が出力されます:

1. ピーク位置（ピークトップのX座標）
1. ピーク高さ (ピークトップのY座標から推定されたバックグラウンド値が差し引かれた値)
1. ピーク半値幅（つまり、full-widths-at-half-maximum (FWHM)と呼ばれる値）

本プログラムにGUIは付属していませんが、ソフトウェアIGOR Proをお持ちであれば、ファイルを開くだけでFigure 2のようなプロットデータを見ることができます.

![Graph1](https://github.com/rtomiyasu/PeakSearch/assets/149344913/63df6fe7-6696-4b3d-a585-fce109f7449c)
```
Figure 2 : 出力された IGOR テキストファイル (IGOR Proで開いたときの表示例)
```

## NEWS
### 2024/1/31
ソースコードの配布ページを OSDN から GitHub に移動しました。

## FAQ
- [ピークサーチプログラムの使い方](#ピークサーチプログラムの使い方)
- [パラメータ調整の方法](#パラメータ調整の方法)
- [バグ報告のお願い](#バグ報告のお願い)
- [謝辞](#謝辞)

### ピークサーチプログラムの使い方
1. 同プログラムを実行するには、以下の３つの入力ファイルを準備する必要があります。（付属の"sample"フォルダに入力例があります。）
    - "*.inp.xml"：計算パラメータの入力ファイル ([例](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/sample1.inp.xml))
    - "cntl.inp.xml"： "*.inp.xml"を含む入出力ファイル名を指定するファイル ([例](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/cntl.inp.xml))
    - ２次元データを入力するためのテキストファイル。現在扱える書式は以下の３つです。
        - [XY](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample3(PF)/sample3.dat)
        - [IGOR](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample2(TOF)/sample2.histogramIgor)
        - [Rietan](https://github.com/rtomiyasu/PeakSearch/blob/main/PeakSearch0_9_99_win/sample/sample1(CharacteristicXrays)/sample1.dat)
        - "XY"形式と"IGOR"形式では、データの３列目にY値（２行目）の観測誤差を入力することが出来ます。３列目に何も書いてなければ、Y値のルートが観測誤差として使用されます。
1. "sample"フォルダの中の一つのフォルダをコピーし、 コピー先のフォルダ内の二つのXMLファイルの中身とファイル名"*.inp.xml"を適宜修正してください。ファイル名"*.inp.xml"を変更した際は、"cntl.inp"のファイル名も併せて修正する必要があります。
1. コマンドプロンプト、またはお使いのOSのターミナルウィンドウを立ち上げ、 上で変更した"cntl.inp"と同じフォルダにカレントフォルダを移動してください。
1. コマンドプロンプトより、PeakSearch.exeの絶対パスを入力して実行します。

### パラメータ調整の方法
より良い結果を得るためには、適宜、[Threshold](#Threshold)と[NumberOfPointsForSGMethod](#NumberOfPointsForSGMethod)を修正するとよいです。
[Alpha2Correction](#Alpha2Correction)には、α2除去に関するパラメータを入力します（α2ピークを含む特性X線データに粉末指数づけを行う前処理のため）。

#### Threshold
ピーク位置 x に検出されたピークを、ピークとして検出・出力するかどうかの判定基準を与えます。
具体的には、ピーク位置のX座標 xにおける、ヒストグラムのY座標 y の推定観測誤差をErr[y]としたとき、(Threshold) × Err[y]より大きなピーク高さを持つピークのみが検出されます。ただし、ここで使用されるピーク高さは、y から推定されたバックグラウンド値が差し引かれた値です。
2次元データのY値の観測誤差として妥当な値が入力されていれば、多くのケースで 3 前後から10の範囲で、Thresholdの最適値が得られると考えられます。

![Graph2](https://github.com/rtomiyasu/PeakSearch/assets/149344913/fd76b9a9-ddb2-4f53-8e38-d11a392b1f10)
```
Figure 3 : 放射光データにおけるピークサーチ結果の例
(Thresholdを変更することで、強度の小さな回折ピークをどの程度検出するかを設定できます。)
```

#### NumberOfPointsForSGMethod
バックグラウンドノイズの検出を抑制するために使用します。
この値が小さいとき、より少ないデータ点数だけが使用される結果として、近似曲線（３次多項式）が局所的な凹凸をより明確に反映します。
Figure 4.1の例は、近似曲線がバックグラウンドノイズを正確に表現してしまうと、ノイズの凹凸をピークとして検出してしまうケースが生じ得ることを示しています。

![Graph3_1](https://github.com/rtomiyasu/PeakSearch/assets/149344913/6455d667-fd3e-4e01-a5b6-ee52a357efcb)
![Graph3_2](https://github.com/rtomiyasu/PeakSearch/assets/149344913/e897d2c0-0021-4213-a4d3-fb987c55b50d)
```
Figure 4.1 : "NumberOfPointsForSGMethod" = 5	Figure 4.2 : "NumberOfPointsForSGMethod" = 25
```

#### Alpha2Correction
特性X線データがα2ピークを含む場合1を、それ以外では0を入力します。
また、Cu Kα2以外のデータでにおいては、Kalpha1WaveLengthおよびKalpha2WaveLengthに入力する波長を、以下の表を参考にご修正ください。

| | Kα1  | Kα2  |
|:---:|---:|---:|
|Ag|0.5594075 |0.563798
|Co|1.788965  |1.792850
|Cr|2.289700  |2.293606
|Cu|1.540562  |1.544398
|Fe|1.936042  |1.939980
|Mo|0.709300  |0.713590

## バグ報告のお願い
コード改善のために、疑わしい事例を見つけられましたら、以下のアドレスまでご連絡頂けると助かります。
その際、"LOG_PEAKSEARCH.txt"を含む全ての入出力ファイルをご添付ください。

- conograph-bugs (at) ml.post.kek.jp

## 謝辞
Conograph projectのために粉末回折データを提供頂いた方々に感謝します。
