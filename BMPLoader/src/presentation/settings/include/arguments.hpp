/**
 * @file arguments.hpp
 * @brief コマンドライン引数の表示・解釈ユーティリティ。
 */
#ifndef __ARGUMENTS_H__
#define __ARGUMENTS_H__

#include <filesystem>

/**
 * @class Arguments
 * @brief コマンドライン引数から設定値を取得します。
 */
class Arguments {
public:
    /**
     * @brief 渡された引数を一覧表示します。
     */
    static void showArguments(int argc, char* argv[]);
    /**
     * @brief 引数を解析します。
     * @retval true 解析成功
     * @retval false 解析失敗
     */
    bool recieveArgument(int argc, char* argv[]);
    /**
     * @brief --load/--l で指定された BMP パスを返します。
     */
    const std::string getLoadBmpPath()const {return m_loadBmpPath;};
private:
    std::string m_loadBmpPath;

    /**
     * @brief BMP 読み込みパスの解析実装。
     */
    bool reciveLoadBmpPath(int argc, char* argv[]);
};

#endif
