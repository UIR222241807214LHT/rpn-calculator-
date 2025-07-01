"""
gui_functions.py
"""

import os
import sys
import math
from ctypes import cdll, c_char_p, c_double
import dearpygui.dearpygui as dpg

# --- 自动适配打包路径加载 DLL ---
if getattr(sys, "frozen", False):
    # PyInstaller 打包后的运行路径
    base_path = sys._MEIPASS
else:
    # 开发模式下的当前路径
    base_path = os.path.dirname(os.path.abspath(__file__))

dll_path = os.path.join(base_path, "rpn_lib.dll")

# 加载 DLL
rpn_lib = cdll.LoadLibrary(dll_path)

# 设置函数返回值类型和参数类型
rpn_lib.evaluate_rpn_c.restype = c_double
rpn_lib.get_last_error.restype = c_char_p
rpn_lib.evaluate_infix_expr_c.argtypes = [c_char_p]
rpn_lib.evaluate_infix_expr_c.restype = c_double


# --- C++ 封装函数 ---
def evaluate_rpn_expr(tokens):
    """
    接收一个中缀表达式的 token 列表，调用 C++ DLL 进行计算。
    参数:
        tokens: list[str]: 是一个字符串列表
    返回:
        float: 计算结果（或 NaN）
    """
    token_array = c_char_p * len(tokens)
    arr = token_array(*(t.encode("utf-8") for t in tokens))
    result = rpn_lib.evaluate_rpn_c(arr, len(tokens))
    if math.isnan(result):
        # 如果是 NaN，说明 C++ 侧出错，取出错误信息
        err = rpn_lib.get_last_error().decode("utf-8", "ignore")
        raise RuntimeError(err)
    return result


# --- 插入到输入框 ---
def insert_to_input(_sender, _app_data, _user_data):
    """
    输入框
    """
    content = _user_data
    current = dpg.get_value("input")

    if content == "C":  # 清空按钮
        dpg.set_value("input", "")
        dpg.set_value("output", "")  # 顺便清空输出
        return
    elif content == "<-":  # 退格按钮
        dpg.set_value("input", current[:-1])  # 移除最后一个字符
        return

    dpg.set_value("input", current + content)


# --- 按钮回调 ---
def on_calculate(_sender, _app_data, _user_data):
    """
    当用户点击“计算”按钮时触发的回调函数。

    功能：
    - 读取输入框中的中缀表达式
    - 解析为 token 列表
    - 调用 C++ DLL 执行计算
    - 将结果或错误信息显示在输出区域

    参数:
        sender (int): 触发该回调的控件 ID（由 DearPyGui 自动传入）
        app_data (Any): 控件传入的数据（未使用）
        user_data (Any): 用户自定义数据（未使用）
    """
    input_expr = dpg.get_value("input").strip()
    if not input_expr:
        dpg.set_value("output", "请输入中缀表达式！")
        return

    try:
        # 调用C++中缀表达式计算接口，传入字符串
        input_bytes = input_expr.encode("utf-8")
        result = rpn_lib.evaluate_infix_expr_c(input_bytes)
        if math.isnan(result):
            err = rpn_lib.get_last_error().decode("utf-8", "ignore")
            raise RuntimeError(err)
        dpg.set_value("output", f"计算结果：{result}")
    except RuntimeError as e:
        dpg.set_value("output", f"错误：{e}")
    except Exception as e:
        dpg.set_value("output", f"未知错误：{e}")


def setup_gui_layout():
    """
    定义一个初始化 GUI 的函数
    这样 main 文件中只需要调用一个函数
    """
    # --- 主题 ---
    with dpg.theme() as button_theme:
        with dpg.theme_component(dpg.mvButton):
            dpg.add_theme_color(
                dpg.mvThemeCol_Button, (100, 150, 255), category=dpg.mvThemeCat_Core
            )  # 背景色
            dpg.add_theme_color(
                dpg.mvThemeCol_ButtonHovered,
                (120, 180, 255),
                category=dpg.mvThemeCat_Core,
            )  # 悬停色
            dpg.add_theme_color(
                dpg.mvThemeCol_ButtonActive,
                (80, 120, 220),
                category=dpg.mvThemeCat_Core,
            )  # 点击色
            dpg.add_theme_color(
                dpg.mvThemeCol_Text, (0, 0, 0), category=dpg.mvThemeCat_Core
            )  # 文字色

    # --- 创建窗口 ---
    with dpg.window(label="计算器", width=500, height=500):

        dpg.add_text("请输入中缀表达式")
        dpg.add_input_text(tag="input", multiline=True, width=485, height=80)

        # --- 更新 6x5 小键盘网格 ---
        button_layout = [
            ["7", "8", "9", "+", "sin"],
            ["4", "5", "6", "-", "cos"],
            ["1", "2", "3", "*", "tan"],
            ["0", "小数点", "空格", "/", "ln"],
            ["(", ")", "pi", "e", "$美中汇率"],
            ["清除", "退格", "计算"],  # 最后一行不满也可以留空
        ]

        # --- 按钮标签与实际输入的映射 ---
        button_input_map = {
            "空格": " ",
            "sin": "sin(",
            "cos": "cos(",
            "tan": "tan(",
            "ln": "log(",
            "小数点": ".",
            "$美中汇率": "$",
            "清除": "C",
            "退格": "<-",
            # 其他默认映射为自身
        }

        with dpg.table(
            header_row=False,
            policy=dpg.mvTable_SizingFixedFit,
            borders_innerH=True,
            borders_innerV=True,
        ):
            # 添加5列表头（可无label）
            for i in range(5):
                dpg.add_table_column(label=f"Column {i+1}")

            for row_idx, row in enumerate(button_layout):
                with dpg.table_row():
                    col_idx = 0
                    while col_idx < len(row):
                        label = row[col_idx]
                        actual_input = button_input_map.get(label, label)
                        btn_tag = f"btn_{row_idx}_{col_idx}"

                        if label == "计算":
                            dpg.add_button(
                                label=label,
                                width=90,
                                height=40,
                                callback=on_calculate,
                                tag=btn_tag,
                            )
                            dpg.bind_item_theme(btn_tag, button_theme)
                            col_idx += 3
                        else:
                            dpg.add_button(
                                label=label,
                                width=90,
                                height=40,
                                callback=insert_to_input,
                                user_data=actual_input,
                                tag=btn_tag,
                            )
                            dpg.bind_item_theme(btn_tag, button_theme)
                            col_idx += 1

        dpg.add_spacer(height=10)
        dpg.add_text("", tag="output")
