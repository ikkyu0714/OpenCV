cmake_minimum_required(VERSION 3.10)
project(opencv_project)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
)

# 実行ファイル一覧（拡張子なし）
set(SOURCES
    main
    prac4_2
    img_read
    img_get
    resolution_get
    img_hist
    img_hist_equalize
    gamma_conversion
    negaposi_conversion
    intensity_adjustment
    contrast_attenuation
    contrast_enhancement
    hough_transform
    hough_trans_extract_shape
    homography_transform
    ransac_plane
    plane_to_homography
    get_pixel_address
)

# まとめてビルド
foreach(target ${SOURCES})
    add_executable(${target} src/${target}.cpp)
    target_link_libraries(${target} ${OpenCV_LIBS})
endforeach()