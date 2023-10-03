// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

use cxx_qt_build::CxxQtBuilder;

fn main()
{
    CxxQtBuilder::new()
        .file("src/muziko.rs")
        .cc_builder(|cc| {
            cc.include("../cpp");
        })
        .build();
}
