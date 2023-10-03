// SPDX-FileCopyrightText: Loren Burkholder
//
// SPDX-License-Identifier: GPL-3.0

#[cxx_qt::bridge]
pub mod muziko
{
    unsafe extern "C++"
    {
        include!(<QAbstractListModel>);

        include!("cxx-qt-lib/qstring.h");
        type QString = cxx_qt_lib::QString;

        include!("cxx-qt-lib/qhash.h");
        type QHash_i32_QByteArray = cxx_qt_lib::QHash<cxx_qt_lib::QHashPair_i32_QByteArray>;

        include!("cxx-qt-lib/qvariant.h");
        type QVariant = cxx_qt_lib::QVariant;

        include!("cxx-qt-lib/qmodelindex.h");
        type QModelIndex = cxx_qt_lib::QModelIndex;

        include!("cxx-qt-lib/qvector.h");
        type QVector_i32 = cxx_qt_lib::QVector<i32>;

        #[cxx_name = "SongsModel"]
        type CxxSongsModel = super::qobject::SongsModel;

        include!("CppHelper.h");
        pub fn createSongsModel() -> *mut CxxSongsModel;
    }

    #[cxx_qt::qobject(
        base = "QAbstractListModel",
        qml_uri = "dev.lorendb.muziko",
        qml_version = "1.0",
        qml_uncreatable)]
    pub struct SongsModel
    {
        #[qproperty]
        instrument: QString,

        songs: Vec<QString>,
    }

    impl Default for SongsModel
    {
        fn default() -> Self
        {
            Self { instrument: QString::from("guitar"), songs: Vec::<QString>::default() }
        }
    }

    #[cxx_qt::qsignals(SongsModel)]
    pub enum Signals<'a>
    {
        #[inherit]
        DataChanged {
            top_left: &'a QModelIndex,
            bottom_right: &'a QModelIndex,
            roles: &'a QVector_i32,
        },
    }

    #[cxx_qt::inherit]
    extern "C++" {
        unsafe fn begin_insert_rows(
            self: Pin<&mut qobject::SongsModel>,
            parent: &QModelIndex,
            first: i32,
            last: i32,
        );
        unsafe fn end_insert_rows(self: Pin<&mut qobject::SongsModel>);

        unsafe fn begin_remove_rows(
            self: Pin<&mut qobject::SongsModel>,
            parent: &QModelIndex,
            first: i32,
            last: i32,
        );
        unsafe fn end_remove_rows(self: Pin<&mut qobject::SongsModel>);

        unsafe fn begin_reset_model(self: Pin<&mut qobject::SongsModel>);
        unsafe fn end_reset_model(self: Pin<&mut qobject::SongsModel>);
    }

    impl qobject::SongsModel
    {
        #[qinvokable]
        pub fn add_song(mut self: Pin<&mut Self>, song: QString)
        {
            unsafe
            {
                let size = self.songs().len() as i32;
                self.as_mut().begin_insert_rows(&QModelIndex::default(), size, size);
                self.as_mut().songs_mut().push(song);
                self.as_mut().end_insert_rows();
            }
        }

        #[qinvokable(cxx_override)]
        pub fn role_names(&self) -> QHash_i32_QByteArray
        {
            let mut roles = QHash_i32_QByteArray::default();
            roles.insert(0, cxx_qt_lib::QByteArray::from("songName"));
            return roles;
        }

        #[qinvokable(cxx_override)]
        pub fn row_count(&self, _parent: &QModelIndex) -> i32
        {
            self.rust().songs.len() as i32
        }

        #[qinvokable(cxx_override)]
        pub fn data(&self, index: &QModelIndex, role: i32) -> QVariant
        {
            if let Some(name) = self.rust().songs.get(index.row() as usize)
            {
                return match role {
                    0 => name.into(),
                    _ => QVariant::default(),
                };
            }
            return QVariant::default();
        }
    }

    #[cxx_qt::qobject(
        qml_uri = "dev.lorendb.muziko",
        qml_version = "1.0",
        qml_singleton)]
    pub struct Muziko
    {
        #[qproperty]
        model: *mut CxxSongsModel,
    }

    impl Default for Muziko
    {
        fn default() -> Self
        {
            Self { model: createSongsModel() }
        }
    }
}
